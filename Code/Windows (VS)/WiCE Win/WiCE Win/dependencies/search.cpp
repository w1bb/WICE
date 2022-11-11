#include "search.hpp"

// ---------------------------------------------
// SearchInfo
// ---------------------------------------------

SearchInfo::SearchInfo()
: startTime(0), stopTime(0),
  depth(0), depthSet(0),
  timeSet(0),
  movesToGo(0), nodes(0), infinite(false),
  quit(false), stopped(false),
  fh(0), fhf(0),
  postThinking(false) {}

// SearchInfo::clearForSearch()
// Only a few select variables have to be reset.
void SearchInfo::clearForSearch() {
	stopped = false;
	nodes = 0;
	fh = 0;
	fhf = 0;
}

// ---------------------------------------------
// Search
// ---------------------------------------------

// [ Variables ]

const int32_t Search::checkUpNodes = 2047;

// [ Methods ]

Search::Search() { // not allowed!
	throw Exception::E_Search_Init_E;
}

// Search::prepare()
// Make sure everything is ready for a search.
void Search::prepare(
	Board& pos,
	SearchInfo& info
) {
	pos.clearForSearch();
	info.clearForSearch();
}

// Search::checkUp()
// At any time, it could be over!
void Search::checkUp(SearchInfo& info) {
	if(info.timeSet == true && getTimeMs() > info.stopTime) {
		info.stopped = true;
	}

	UCI::readInput(info);
}

// Search::searchPosition()
// Iterative deepening.
void Search::searchPosition(
	Board& pos,
	SearchInfo& info,
	WiCEOptions& options,
	PolyBook& polyBook
) {
	int bestMove = NOMOVE;
	int bestScore = -WiCE_INF;
	int currentDepth = 0;
	int pvMoves = 0;
	int pvNum = 0;

	prepare(pos, info);
	if(options.useBook) {
		bestMove = polyBook.getBookMove(pos);
	}

	if(bestMove == NOMOVE) {
		for(currentDepth = 1; currentDepth <= info.depth; ++currentDepth) {
			bestScore = alphabeta(
				-WiCE_INF,
				WiCE_INF,
				currentDepth,
				pos,
				info,
				true
			);
			
			if(info.stopped) break;
			
			pvMoves = pos.getPVLine(currentDepth);
			bestMove = pos.pvArray[0];
			
			if(info.postThinking && options.mode == MODE_Console) {
				printf("Depth:%d   score:%d   move:%s   nodes:%" PRId64 " ",
					currentDepth,
					bestScore,
					Move(bestMove).toString().c_str(),
					info.nodes
				);

				pvMoves = pos.getPVLine(currentDepth);
				printf("\npv:");
				for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
					printf(" %s", Move(pos.pvArray[pvNum]).toString().c_str());
				}
				printf("\n");
			} else if(options.mode == MODE_UCI) {
				printf("info score cp %d depth %d nodes %" PRId64 " time %" PRIu64 " pv",
					bestScore,
					currentDepth,
					info.nodes,
					getTimeMs() - info.startTime
				);
				for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
					printf(" %s", Move(pos.pvArray[pvNum]).toString().c_str());
				}
				printf("\n");
			} else if(options.mode == MODE_xboard && info.postThinking) {
				printf("%d %d %" PRIu64 " %" PRId64 "",
					currentDepth,
					bestScore,
					(getTimeMs() - info.startTime) / U64(10),
					info.nodes
				);
			}
		}
	}

	if(options.mode == MODE_Console) {
		printf("\n\nBest Move: %s (depth:%d | score:%d)",
			Move(bestMove).toString().c_str(),
			info.depth,
			bestScore
		);
	} else if(options.mode == MODE_UCI) {
		printf("bestmove %s\n",
			Move(bestMove).toString().c_str()
		);
	} else if(options.mode == MODE_xboard) {
		printf("\n\n***!! WiCE makes move %s !!***\n",
			Move(bestMove).toString().c_str()
		);
		pos.makeMove(bestMove);
		pos.print();
	}
}

// Search::alphabeta()
// The bread of the searching algorithm.
const int32_t Search::alphabeta(
	int32_t alpha,
	const int32_t beta,
	int32_t depth,
	Board& pos,
	SearchInfo& info,
	const bool doNull
) {
	ASSERT(pos.checkBoard());
	if(depth == 0)
		return quiescence(alpha, beta, pos, info);

	if((info.nodes & checkUpNodes) == 0) {
		checkUp(info);
	}
	info.nodes++;

	if(pos.isRepetition() || (pos.fiftyMove >= 100 && pos.ply)) {
		return 0;
	}
	if(pos.ply > MAX_DEPTH - 1) {
		return pos.eval();
	}
	
	bool inCheck = pos.sqAttacked(
		pos.KingSq[pos.side],
		(pos.side == WHITE) ? BLACK : WHITE
	);

	if(inCheck) ++depth;

	int32_t score = -WiCE_INF;
	int32_t pvMove = NOMOVE;

	if(pos.probeHashEntry(
		pvMove,
		score,
		alpha,
		beta,
		depth
	)) {
		pos.hashTable->cut++;
		return score;
	}

	if(doNull && !inCheck && pos.ply && (pos.bigPce[pos.side] > 0) && depth >= 4) {
		pos.makeNullMove();
		score = -alphabeta(
			-beta,
			-beta + 1,
			depth - 4,
			pos,
			info,
			false
		);
		pos.takeNullMove();
		if(info.stopped)
			return 0;
		if(score >= beta && score > -ISMATE && score < ISMATE) 
			return beta;
	}

	MoveList list(pos);
      
    int32_t moveNum = 0;
	int32_t legal = 0;
	int32_t oldAlpha = alpha;
	int32_t bestMove = NOMOVE;

	score = -WiCE_INF;
	int32_t bestScore = -WiCE_INF;

	if(pvMove != NOMOVE) {
		for(moveNum = 0; moveNum < list.count; ++moveNum) {
			if( list.moves[moveNum].move == pvMove) {
				list.moves[moveNum].score = 2000000;
				break;
			}
		}
	}
	
	for(moveNum = 0; moveNum < list.count; ++moveNum) {	
		list.pickNextMove(moveNum);	
		
        if (!pos.makeMove(list.moves[moveNum].move))
            continue;
        
		legal++;
		score = -alphabeta(
			-beta,
			-alpha,
			depth - 1,
			pos,
			info,
			true
		);
        pos.takeMove();
		
        if(info.stopped) return 0;

        if(score > bestScore) {
        	bestScore = score;
        	bestMove = list.moves[moveNum].move;
        	if(score > alpha) {
				if(score >= beta) {
					if(legal == 1) {
						info.fhf++;
					}
					info.fh++;
					if(!(list.moves[moveNum].move & MFLAGCAP)) {
						pos.searchKillers[1][pos.ply] = pos.searchKillers[0][pos.ply];
						pos.searchKillers[0][pos.ply] = list.moves[moveNum].move;
					}
					pos.storeHashEntry(
						bestMove,
						beta,
						HFBETA,
						depth
					);
					return beta;
				}
				alpha = score;
				if(!(list.moves[moveNum].move & MFLAGCAP)) {
					pos.searchHistory[pos.pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] += depth;
				}
			}
        }

		
    }
    if(legal == 0) {
		if(inCheck) {
			return -MATE + pos.ply;
		} else {
			return 0;
		}
	}

	if(alpha != oldAlpha) {
		pos.storeHashEntry(
			bestMove,
			bestScore,
			HFEXACT,
			depth
		);
	} else {
		pos.storeHashEntry(
			bestMove,
			alpha,
			HFALPHA,
			depth
		);
	}
	
	return alpha;
}

// Search::quiescence()
// Revokes the horizon effect
const int32_t Search::quiescence(
	int32_t alpha,
	const int32_t beta,
	Board& pos,
	SearchInfo& info
) {
	ASSERT(pos.checkBoard());

	if((info.nodes & checkUpNodes) == 0) {
		checkUp(info);
	}

	++info.nodes;

	if(pos.isRepetition() || pos.fiftyMove >= 100) {
		return 0;
	}
	if(pos.ply > MAX_DEPTH - 1) {
		return pos.eval();
	}
	
	int32_t score = pos.eval();
	if(score >= beta) {
		return beta;
	}
	if(score > alpha) {
		alpha = score;
	}
	
	MoveList list;
	list.generateCapsFrom(pos);
      
    int32_t moveNum = 0;
	int32_t legal = 0;
	score = -WiCE_INF;

	for(moveNum = 0; moveNum < list.count; ++moveNum) {
		list.pickNextMove(moveNum);
		
        if (!pos.makeMove(list.moves[moveNum].move))  {
            continue;
        }
        
		legal++;
		score = -quiescence( -beta, -alpha, pos, info);		
        pos.takeMove();
		
        if(info.stopped) return 0;

		if(score > alpha) {
			if(score >= beta) {
				if(legal == 1) {
					info.fhf++;
				}
				info.fh++;
				return beta;
			}
			alpha = score;
		}		
    }

    return alpha;
}