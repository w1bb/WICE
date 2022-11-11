#include "move.hpp"

// ---------------------------------------------
// Move
// ---------------------------------------------

// [ Variables / Constants ]

const int32_t Move::victimScore[13] = {
	0,
	100, 200, 300, 400, 500, 600,
	100, 200, 300, 400, 500, 600
};

const int32_t Move::mvvLvaScores[13][13] = {
	105, 104, 103, 102, 101, 100, 105, 104, 103, 102, 101, 100,
	205, 204, 203, 202, 201, 200, 205, 204, 203, 202, 201, 200,
	305, 304, 303, 302, 301, 300, 305, 304, 303, 302, 301, 300,
	405, 404, 403, 402, 401, 400, 405, 404, 403, 402, 401, 400,
	505, 504, 503, 502, 501, 500, 505, 504, 503, 502, 501, 500,
	605, 604, 603, 602, 601, 600, 605, 604, 603, 602, 601, 600,
	105, 104, 103, 102, 101, 100, 105, 104, 103, 102, 101, 100,
	205, 204, 203, 202, 201, 200, 205, 204, 203, 202, 201, 200,
	305, 304, 303, 302, 301, 300, 305, 304, 303, 302, 301, 300,
	405, 404, 403, 402, 401, 400, 405, 404, 403, 402, 401, 400,
	505, 504, 503, 502, 501, 500, 505, 504, 503, 502, 501, 500,
	605, 604, 603, 602, 601, 600, 605, 604, 603, 602, 601, 600
};

const int32_t Move::EP_Flag = 0x40000;
const int32_t Move::PS_Flag = 0x80000;
const int32_t Move::CA_Flag = 0x1000000;

// [ Methods ]

Move::Move()
: move(0), score(0) {}

Move::Move(const int32_t _move)
: move(_move), score(0) {}

const int32_t Move::getFrom() {
	return move & 0x7f;
}

const int32_t Move::getTo() {
	return (move >> 7) & 0x7f;
}

const int32_t Move::getCaptured() {
	return (move >> 14) & 0xf;
}

const int32_t Move::getPromoted() {
	return (move >> 20) & 0xf;
}

const int32_t Move::getEP() {
	return move & EP_Flag;
}

const int32_t Move::getPS() {
	return move & PS_Flag;
}

const int32_t Move::getCA() {
	return move & CA_Flag;
}

const bool Move::wasCaptured() {
	return move & 0x7C000;
}

const bool Move::wasPromoted() {
	return move & 0xF00000;
}

const string Move::toString() {
	string result = "";
	int32_t ff = FilesBrd[this->getFrom()];
	int32_t rf = RanksBrd[this->getFrom()];
	int32_t ft = FilesBrd[this->getTo()];
	int32_t rt = RanksBrd[this->getTo()];

	result.push_back(ff+'a');
	result.push_back(rf+'1');
	result.push_back(ft+'a');
	result.push_back(rt+'1');

	int32_t promoted = this->getPromoted();
	if(promoted) {
		if(IsKn(promoted))
			result.push_back('n');
		else if(IsRQ(promoted) && !IsBQ(promoted))
			result.push_back('r');
		else if(!IsRQ(promoted) && IsBQ(promoted))
			result.push_back('b');
		else
			result.push_back('q');
	}
	return result;
}

void Move::print() {
	printf("\n%s", toString().c_str());
}

// ---------------------------------------------
// MoveList
// ---------------------------------------------

// [ Variables / Constants ]

const int32_t MoveList::LoopSlidePce[8] = {
	wB, wR, wQ, 0, bB, bR, bQ, 0
};
const int32_t MoveList::LoopNonSlidePce[6] = {
 	wN, wK, 0, bN, bK, 0
};
const int32_t MoveList::LoopSlideIndex[2] = {
	0, 4
};
const int32_t MoveList::LoopNonSlideIndex[2] = {
	0, 3
};

const int32_t MoveList::PceDir[13][8] = {
	{  0,   0,   0,   0,  0,   0,  0     },
	{  0,   0,   0,   0,  0,   0,  0     },
	{ -8, -19, -21, -12,  8,  19, 21, 12 },
	{ -9, -11,  11,   9,  0,   0,  0,  0 },
	{ -1, -10,	 1,  10,  0,   0,  0,  0 },
	{ -1, -10,	 1,  10, -9, -11, 11,  9 },
	{ -1, -10,	 1,  10, -9, -11, 11,  9 },
	{  0,   0,   0,   0,  0,   0,  0     },
	{ -8, -19, -21, -12,  8,  19, 21, 12 },
	{ -9, -11,  11,   9,  0,   0,  0,  0 },
	{ -1, -10,	 1,  10,  0,   0,  0,  0 },
	{ -1, -10,	 1,  10, -9, -11, 11,  9 },
	{ -1, -10,	 1,  10, -9, -11, 11,  9 }
};

const int32_t MoveList::NumDir[13] = {
 	0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
};

// [ Methods ]
MoveList::MoveList()
: count(0) {}

MoveList::MoveList(Board& board)
: count(0) {
	generateFrom(board);
}

void MoveList::pickNextMove(const int32_t moveNum) {
	Move temp;
	int32_t i = 0;
	int32_t bestScore = 0; 
	int32_t bestNum = moveNum;
	
	for (i = moveNum; i < count; ++i) {
		if (moves[i].score > bestScore) {
			bestScore = moves[i].score;
			bestNum = i;
		}
	}

	temp = moves[moveNum];
	moves[moveNum] = moves[bestNum];
	moves[bestNum] = temp;
}

void MoveList::addQuietMove(
	Board& pos,
	const int32_t move
) {
	ASSERT(isSqOnBoard(FROMSQ(move)));
	ASSERT(isSqOnBoard(TOSQ(move)));

	moves[count].move = move;
	
	if(pos.searchKillers[0][pos.ply] == move) {	
		moves[count].score = 900000;
	} else if(pos.searchKillers[1][pos.ply] == move) {	
		moves[count].score = 800000;
	} else {	
		moves[count].score = pos.searchHistory[pos.pieces[FROMSQ(move)]][TOSQ(move)];
	}
	++count;
}

void MoveList::addCaptureMove(
	Board& pos,
	const int32_t move
) {
	ASSERT(isSqOnBoard(FROMSQ(move)));
	ASSERT(isSqOnBoard(TOSQ(move)));
	ASSERT(isPieceValid(CAPTURED(move)));
	
	moves[count].move = move;
	moves[count].score = Move::mvvLvaScores[CAPTURED(move)][pos.pieces[FROMSQ(move)]] + 1000000;
	++count;
}

void MoveList::addEnPassantMove(
	Board& pos,
	const int32_t move
) {
	ASSERT(isSqOnBoard(FROMSQ(move)));
	ASSERT(isSqOnBoard(TOSQ(move)));
	
	moves[count].move = move;
	moves[count].score = 105 + 1000000;
	++count;
}

void MoveList::addWhitePawnCapMove(
	Board& pos,
	const int32_t from,
	const int32_t to,
	const int32_t cap
) {
	ASSERT(isPieceValidEmpty(cap));
	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	if(RanksBrd[from] == RANK_7) {
		addCaptureMove(pos, MOVE(from,to,cap,wQ, 0));
		addCaptureMove(pos, MOVE(from,to,cap,wR, 0));
		addCaptureMove(pos, MOVE(from,to,cap,wB, 0));
		addCaptureMove(pos, MOVE(from,to,cap,wN, 0));
	} else {
		addCaptureMove(pos, MOVE(from,to,cap,EMPTY, 0));
	}
}

void MoveList::addWhitePawnMove(
	Board& pos,
	const int32_t from,
	const int32_t to
) {
	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	if(RanksBrd[from] == RANK_7) {
		addQuietMove(pos, MOVE(from,to,EMPTY,wQ, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,wR, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,wB, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,wN, 0));
	} else {
		addQuietMove(pos, MOVE(from,to,EMPTY,EMPTY, 0));
	}
}

void MoveList::addBlackPawnCapMove(
	Board& pos,
	const int32_t from,
	const int32_t to,
	const int32_t cap
) {
	ASSERT(isPieceValidEmpty(cap));
	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	if(RanksBrd[from] == RANK_2) {
		addCaptureMove(pos, MOVE(from,to,cap,bQ, 0));
		addCaptureMove(pos, MOVE(from,to,cap,bR, 0));
		addCaptureMove(pos, MOVE(from,to,cap,bB, 0));
		addCaptureMove(pos, MOVE(from,to,cap,bN, 0));
	} else {
		addCaptureMove(pos, MOVE(from,to,cap,EMPTY, 0));
	}
}

void MoveList::addBlackPawnMove(
	Board& pos,
	const int32_t from,
	const int32_t to
) {
	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	if(RanksBrd[from] == RANK_2) {
		addQuietMove(pos, MOVE(from,to,EMPTY,bQ, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,bR, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,bB, 0));
		addQuietMove(pos, MOVE(from,to,EMPTY,bN, 0));
	} else {
		addQuietMove(pos, MOVE(from,to,EMPTY,EMPTY, 0));
	}
}

void MoveList::generateFrom(Board& pos) {
	ASSERT(pos.checkBoard());

	count = 0;
	
	int32_t pce = EMPTY;
	int32_t side = pos.side;
	int32_t sq = 0; int t_sq = 0;
	int32_t pceNum = 0;
	int32_t dir = 0;
	int32_t index = 0;
	int32_t pceIndex = 0;
	
	if(side == WHITE) {
		for(pceNum = 0; pceNum < pos.pceNum[wP]; ++pceNum) {
			sq = pos.pList[wP][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			if(pos.pieces[sq + 10] == EMPTY) {
				addWhitePawnMove(pos, sq, sq+10);
				if(RanksBrd[sq] == RANK_2 && pos.pieces[sq + 20] == EMPTY) {
					addQuietMove(pos, MOVE(sq,(sq+20),EMPTY,EMPTY,MFLAGPS));
				}
			} 
			
			if(!SQOFFBOARD(sq + 9) && getPieceCol[pos.pieces[sq + 9]] == BLACK) {
				addWhitePawnCapMove(pos, sq, sq+9, pos.pieces[sq + 9]);
			}  
			if(!SQOFFBOARD(sq + 11) && getPieceCol[pos.pieces[sq + 11]] == BLACK) {
				addWhitePawnCapMove(pos, sq, sq+11, pos.pieces[sq + 11]);
			} 
			
			if(pos.enPas != NO_SQ) {
				if(sq + 9 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq + 9,EMPTY,EMPTY,MFLAGEP));
				} 
				if(sq + 11 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq + 11,EMPTY,EMPTY,MFLAGEP));
				}
			}
		}		
		if(pos.castlePerm & WKCA) {
			if(pos.pieces[F1] == EMPTY && pos.pieces[G1] == EMPTY) {
				if(!pos.sqAttacked(E1,BLACK) && !pos.sqAttacked(F1,BLACK) ) {
					addQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA));
				}
			}
		}
		if(pos.castlePerm & WQCA) {
			if(pos.pieces[D1] == EMPTY && pos.pieces[C1] == EMPTY && pos.pieces[B1] == EMPTY) {
				if(!pos.sqAttacked(E1,BLACK) && !pos.sqAttacked(D1,BLACK) ) {
					addQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA));
				}
			}
		}
	} else {
		for(pceNum = 0; pceNum < pos.pceNum[bP]; ++pceNum) {
			sq = pos.pList[bP][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			if(pos.pieces[sq - 10] == EMPTY) {
				addBlackPawnMove(pos, sq, sq-10);
				if(RanksBrd[sq] == RANK_7 && pos.pieces[sq - 20] == EMPTY) {
					addQuietMove(pos, MOVE(sq,(sq-20),EMPTY,EMPTY,MFLAGPS));
				}
			} 
			
			if(!SQOFFBOARD(sq - 9) && getPieceCol[pos.pieces[sq - 9]] == WHITE) {
				addBlackPawnCapMove(pos, sq, sq-9, pos.pieces[sq - 9]);
			} 
			
			if(!SQOFFBOARD(sq - 11) && getPieceCol[pos.pieces[sq - 11]] == WHITE) {
				addBlackPawnCapMove(pos, sq, sq-11, pos.pieces[sq - 11]);
			} 
			if(pos.enPas != NO_SQ) {
				if(sq - 9 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq - 9,EMPTY,EMPTY,MFLAGEP));
				} 
				if(sq - 11 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq - 11,EMPTY,EMPTY,MFLAGEP));
				}
			}
		}
		if(pos.castlePerm &  BKCA) {
			if(pos.pieces[F8] == EMPTY && pos.pieces[G8] == EMPTY) {
				if(!pos.sqAttacked(E8,WHITE) && !pos.sqAttacked(F8,WHITE) ) {
					addQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA));
				}
			}
		}
		if(pos.castlePerm &  BQCA) {
			if(pos.pieces[D8] == EMPTY && pos.pieces[C8] == EMPTY && pos.pieces[B8] == EMPTY) {
				if(!pos.sqAttacked(E8,WHITE) && !pos.sqAttacked(D8,WHITE) ) {
					addQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA));
				}
			}
		}
	}
	
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while( pce != 0) {
		ASSERT(isPieceValid(pce));		
		
		for(pceNum = 0; pceNum < pos.pceNum[pce]; ++pceNum) {
			sq = pos.pList[pce][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;
				
				while(!SQOFFBOARD(t_sq)) {
					if(pos.pieces[t_sq] != EMPTY) {
						if( getPieceCol[pos.pieces[t_sq]] == ((side == WHITE) ? BLACK : WHITE)) {
							addCaptureMove(pos, MOVE(sq, t_sq, pos.pieces[t_sq], EMPTY, 0));
						}
						break;
					}	
					addQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0));
					t_sq += dir;
				}
			}
		}
		
		pce = LoopSlidePce[pceIndex++];
	}

	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];
	
	while( pce != 0) {
		ASSERT(isPieceValid(pce));
		
		for(pceNum = 0; pceNum < pos.pceNum[pce]; ++pceNum) {
			sq = pos.pList[pce][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;
				
				if(SQOFFBOARD(t_sq)) {				    
					continue;
				}
				if(pos.pieces[t_sq] != EMPTY) {
					if( getPieceCol[pos.pieces[t_sq]] == ((side == WHITE) ? BLACK : WHITE)) {
						addCaptureMove(pos, MOVE(sq, t_sq, pos.pieces[t_sq], EMPTY, 0));
					}
					continue;
				}	
				addQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0));
			}
		}
		pce = LoopNonSlidePce[pceIndex++];
	}
}

void MoveList::generateCapsFrom(Board& pos) {
	ASSERT(pos.checkBoard());

	count = 0;
	
	int32_t pce = EMPTY;
	int32_t side = pos.side;
	int32_t sq = 0; int t_sq = 0;
	int32_t pceNum = 0;
	int32_t dir = 0;
	int32_t index = 0;
	int32_t pceIndex = 0;
	
	if(side == WHITE) {
		for(pceNum = 0; pceNum < pos.pceNum[wP]; ++pceNum) {
			sq = pos.pList[wP][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			if(!SQOFFBOARD(sq + 9) && getPieceCol[pos.pieces[sq + 9]] == BLACK) {
				addWhitePawnCapMove(pos, sq, sq+9, pos.pieces[sq + 9]);
			}  
			if(!SQOFFBOARD(sq + 11) && getPieceCol[pos.pieces[sq + 11]] == BLACK) {
				addWhitePawnCapMove(pos, sq, sq+11, pos.pieces[sq + 11]);
			} 
			
			if(pos.enPas != NO_SQ) {
				if(sq + 9 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq + 9,EMPTY,EMPTY,MFLAGEP));
				} 
				if(sq + 11 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq + 11,EMPTY,EMPTY,MFLAGEP));
				}
			}
		}		
	} else {
		for(pceNum = 0; pceNum < pos.pceNum[bP]; ++pceNum) {
			sq = pos.pList[bP][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			if(!SQOFFBOARD(sq - 9) && getPieceCol[pos.pieces[sq - 9]] == WHITE) {
				addBlackPawnCapMove(pos, sq, sq-9, pos.pieces[sq - 9]);
			} 
			
			if(!SQOFFBOARD(sq - 11) && getPieceCol[pos.pieces[sq - 11]] == WHITE) {
				addBlackPawnCapMove(pos, sq, sq-11, pos.pieces[sq - 11]);
			} 
			if(pos.enPas != NO_SQ) {
				if(sq - 9 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq - 9,EMPTY,EMPTY,MFLAGEP));
				} 
				if(sq - 11 == pos.enPas) {
					addEnPassantMove(pos, MOVE(sq,sq - 11,EMPTY,EMPTY,MFLAGEP));
				}
			}
		}
	}
	
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while( pce != 0) {
		ASSERT(isPieceValid(pce));		
		
		for(pceNum = 0; pceNum < pos.pceNum[pce]; ++pceNum) {
			sq = pos.pList[pce][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;
				
				while(!SQOFFBOARD(t_sq)) {
					if(pos.pieces[t_sq] != EMPTY) {
						if( getPieceCol[pos.pieces[t_sq]] == ((side == WHITE) ? BLACK : WHITE)) {
							addCaptureMove(pos, MOVE(sq, t_sq, pos.pieces[t_sq], EMPTY, 0));
						}
						break;
					}	
					t_sq += dir;
				}
			}
		}
		
		pce = LoopSlidePce[pceIndex++];
	}

	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];
	
	while( pce != 0) {
		ASSERT(isPieceValid(pce));
		
		for(pceNum = 0; pceNum < pos.pceNum[pce]; ++pceNum) {
			sq = pos.pList[pce][pceNum];
			ASSERT(isSqOnBoard(sq));
			
			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;
				
				if(SQOFFBOARD(t_sq)) {				    
					continue;
				}
				if(pos.pieces[t_sq] != EMPTY) {
					if( getPieceCol[pos.pieces[t_sq]] == ((side == WHITE) ? BLACK : WHITE)) {
						addCaptureMove(pos, MOVE(sq, t_sq, pos.pieces[t_sq], EMPTY, 0));
					}
					continue;
				}	
			}
		}
		pce = LoopNonSlidePce[pceIndex++];
	}
}

const string MoveList::toString() {
	string result = "";
	for(int32_t i = 0; i < count; ++i)
		result += "Move " + to_string(i+1) + " | " + moves[i].toString() + "\n";
	if(count) result.pop_back();
	return result;
}

void MoveList::print() {
	printf("\n%s", toString().c_str());
}