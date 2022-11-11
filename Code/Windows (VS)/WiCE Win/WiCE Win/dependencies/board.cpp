#include "board.hpp"

// ---------------------------------------------
// Board
// ---------------------------------------------

// [ Loading Methods ]

// Board::parseFEN()
// Parses a FEN, also know as Forsyth–Edwards
// Notation.
void Board::parseFEN(char * fen) {
	ASSERT(fen != NULL);
	
	int32_t rank = RANK_8;
    int32_t file = FILE_A;
    int32_t piece = 0;
    int32_t count = 0;
    int32_t i = 0; 
	int32_t sq64 = 0; 
	int32_t sq120 = 0;
	
	reset();
	
	while ((rank >= RANK_1) && *fen) {
	    count = 1;
		switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;
            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;
            case '/': case ' ':
                rank--;
                file = FILE_A;
                fen++;
                continue;
            default:
                throw Exception::E_Board_FEN_E;
        }
		
		for (i = 0; i < count; i++) {			
            sq64 = rank * 8 + file;
			sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pieces[sq120] = piece;
            }
			file++;
        }
		fen++;
	}
	
	ASSERT(*fen == 'w' || *fen == 'b');
	
	side = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;
	
	for (i = 0; i < 4; i++) {
        if (*fen == ' ') {
            break;
        }		
		switch(*fen) {
			case 'K': castlePerm |= WKCA; break;
			case 'Q': castlePerm |= WQCA; break;
			case 'k': castlePerm |= BKCA; break;
			case 'q': castlePerm |= BQCA; break;
			default:	     break;
        }
		fen++;
	}
	fen++;
	
	ASSERT(castlePerm>=0 && castlePerm <= 15);
	
	if (*fen != '-') {        
		file = fen[0] - 'a';
		rank = fen[1] - '1';
		
		ASSERT(file>=FILE_A && file <= FILE_H);
		ASSERT(rank>=RANK_1 && rank <= RANK_8);
		
		enPas = FR2SQ(file,rank);		
    }
	
	posKey = Hash::generatePosKey(this);
	updateListMaterial();
}

// Board::mirrorBoard()
// Convert the board into a reversed replica.
// Very useful in testing.
void Board::mirrorBoard() {
	int32_t tempPiecesArray[64];
    const Alliance tempSide = (side == WHITE) ? BLACK : WHITE;

	static const int32_t SwapPiece[13] = {
		EMPTY,
		bP, bN, bB, bR, bQ, bK,
		wP, wN, wB, wR, wQ, wK
	};

    int32_t tempCastlePerm = 0;
    int32_t tempEnPas = NO_SQ;
	int32_t sq, tp;
	
    if(castlePerm & WKCA) tempCastlePerm |= BKCA;
    if(castlePerm & WQCA) tempCastlePerm |= BQCA;
    if(castlePerm & BKCA) tempCastlePerm |= WKCA;
    if(castlePerm & BQCA) tempCastlePerm |= WQCA;
	
	if(enPas != NO_SQ)
        tempEnPas = SQ120(Mirror64[SQ64(enPas)]);
    for(sq = 0; sq < 64; sq++)
        tempPiecesArray[sq] = pieces[SQ120(Mirror64[sq])];

    reset();
	for(sq = 0; sq < 64; sq++) {
        tp = SwapPiece[tempPiecesArray[sq]];
        pieces[SQ120(sq)] = tp;
    }
	
	side = tempSide;
    castlePerm = tempCastlePerm;
    enPas = tempEnPas;

    posKey = Hash::generatePosKey(this);
    updateListMaterial();

    ASSERT(checkBoard());
}

// [ Maintenance Methods ]

// Board::checkBoard()
// Checks if the board configuration is valid.
// Used in tests, as it is very time consuming.
const bool Board::checkBoard() {

	int32_t t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int32_t t_bigPce[2] = { 0, 0 };
	int32_t t_majPce[2] = { 0, 0 };
	int32_t t_minPce[2] = { 0, 0 };
	int32_t t_material[2] = { 0, 0 };
	
	int32_t sq64, t_piece, t_pce_num, sq120, color;
	
	BitBoard t_pawns[3] = {0, 0, 0};
	
	t_pawns[WHITE] = pawns[WHITE];
	t_pawns[BLACK] = pawns[BLACK];
	t_pawns[BOTH] = pawns[BOTH];
	
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		for(t_pce_num = 0; t_pce_num < pceNum[t_piece]; ++t_pce_num) {
			sq120 = pList[t_piece][t_pce_num];
			ASSERT(pieces[sq120]==t_piece);
		}	
	}
	
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120(sq64);
		t_piece = pieces[sq120];
		t_pceNum[t_piece]++;
		color = getPieceCol[t_piece];
		if(isPieceBig[t_piece] == true) t_bigPce[color]++;
		if(isPieceMin[t_piece] == true) t_minPce[color]++;
		if(isPieceMaj[t_piece] == true) t_majPce[color]++;
		
		t_material[color] += getPieceVal[t_piece];
	}
	
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		ASSERT(t_pceNum[t_piece]==pceNum[t_piece]);	
	}
	
	ASSERT(t_pawns[WHITE].countBits() == pceNum[wP]);
	ASSERT(t_pawns[BLACK].countBits() == pceNum[bP]);
	ASSERT(t_pawns[BOTH].countBits() == (pceNum[bP] + pceNum[wP]));
	
	while(t_pawns[WHITE].get()) {
		sq64 = t_pawns[WHITE].popBit();
		ASSERT(pieces[SQ120(sq64)] == wP);
	}
	
	while(t_pawns[BLACK].get()) {
		sq64 = t_pawns[BLACK].popBit();
		ASSERT(pieces[SQ120(sq64)] == bP);
	}
	
	while(t_pawns[BOTH].get()) {
		sq64 = t_pawns[BOTH].popBit();
		ASSERT( (pieces[SQ120(sq64)] == bP) || (pieces[SQ120(sq64)] == wP) );
	}
	
	ASSERT(t_material[WHITE]==material[WHITE] && t_material[BLACK]==material[BLACK]);
	ASSERT(t_minPce[WHITE]==minPce[WHITE] && t_minPce[BLACK]==minPce[BLACK]);
	ASSERT(t_majPce[WHITE]==majPce[WHITE] && t_majPce[BLACK]==majPce[BLACK]);
	ASSERT(t_bigPce[WHITE]==bigPce[WHITE] && t_bigPce[BLACK]==bigPce[BLACK]);	
	
	ASSERT(side==WHITE || side==BLACK);
	ASSERT(Hash::generatePosKey(this)==posKey);
	
	ASSERT(enPas==NO_SQ || ( RanksBrd[enPas]==RANK_6 && side == WHITE)
		 || ( RanksBrd[enPas]==RANK_3 && side == BLACK));
	
	ASSERT(pieces[KingSq[WHITE]] == wK);
	ASSERT(pieces[KingSq[BLACK]] == bK);
		 
	return true;	
}

// Board::updateListMaterial()
// As the name sugests, the method updates the material
// list.
void Board::updateListMaterial() {
	int32_t piece, sq, i, color;
	
	for(i = 0; i < 120; ++i) {
		sq = i;
		piece = pieces[i];
		if(piece!=OFFBOARD && piece!= EMPTY) {
			color = getPieceCol[piece];
			
		    if(isPieceBig[piece] == true) bigPce[color]++;
		    if(isPieceMin[piece] == true) minPce[color]++;
		    if(isPieceMaj[piece] == true) majPce[color]++;
			
			material[color] += getPieceVal[piece];
			
			pList[piece][pceNum[piece]] = sq;
			pceNum[piece]++;
			
			if(piece==wK) KingSq[WHITE] = sq;
			if(piece==bK) KingSq[BLACK] = sq;	
			
			if(piece==wP) {
				pawns[WHITE].setBit(SQ64(sq));
				pawns[BOTH].setBit(SQ64(sq));
			} else if(piece==bP) {
				pawns[BLACK].setBit(SQ64(sq));
				pawns[BOTH].setBit(SQ64(sq));
			}
		}
	}
}

// Board::reset()
// A fresh start if you will.
void Board::reset() {
	int32_t i = 0;
	
	for(i = 0; i < 120; ++i) {
		pieces[i] = OFFBOARD;
	} for(i = 0; i < 64; ++i) {
		pieces[SQ120(i)] = EMPTY;
	}
	
	for(i = 0; i < 2; ++i) {
		bigPce[i] = 0;
		majPce[i] = 0;
		minPce[i] = 0;
		material[i] = 0;		
	}
	
	for(i = 0; i < 3; ++i) {
		pawns[i] = 0ULL;
	} for(i = 0; i < 13; ++i) {
		pceNum[i] = 0;
	}
	
	KingSq[WHITE] = KingSq[BLACK] = NO_SQ;
	
	side = BOTH;
	enPas = NO_SQ;
	fiftyMove = 0;
	
	ply = 0;
	hisPly = 0;
	
	castlePerm = 0;
	posKey = 0;
}

// [ Hashing Methods ]

void Board::hashPce(
	const int32_t pce,
	const int32_t sq
) {
	posKey ^= Hash::PieceKeys[pce][sq];
}

void Board::hashCA() {
	posKey ^= Hash::CastleKeys[castlePerm];
}

void Board::hashSide() {
	posKey ^= Hash::SideKey;
}

void Board::hashEP() {
	posKey ^= Hash::PieceKeys[EMPTY][enPas];
}

// [ Piece Methods ]

void Board::clearPiece(
	const int32_t sq
) {
	ASSERT(isSqOnBoard(sq));
	int32_t pce = pieces[sq];
	ASSERT(isPieceValid(pce));

	int32_t col = getPieceCol(pce);
	int32_t t_pceNum = -1;

	hashPce(pce, sq);
	pieces[sq] = EMPTY;
	material[col] -= getPieceVal(pce);

	if(isPieceBig(pce)) {
		--bigPce[col];
		if(isPieceMaj(pce)) {
			--majPce[col];
		} else {
			--minPce[col];
		}
	} else {
		pawns[col].clearBit(SQ64(sq));
		pawns[BOTH].clearBit(SQ64(sq));
	}

	for(int32_t i = 0; i < pceNum[pce]; ++i) {
		if(pList[pce][i] == sq) {
			t_pceNum = i;
			break;
		}
	}

	ASSERT(t_pceNum != -1);

	--pceNum[pce];
	pList[pce][t_pceNum] = pList[pce][pceNum[pce]];
}

void Board::addPiece(
	const int32_t sq,
	const int32_t pce
) {
	ASSERT(isSqOnBoard(sq));
	ASSERT(isPieceValid(pce));
	int32_t col = getPieceCol(pce);

	hashPce(pce, sq);
	pieces[sq] = pce;

	if(isPieceBig(pce)) {
		++bigPce[col];
		if(isPieceMaj(pce)) {
			++majPce[col];
		} else {
			++minPce[col];
		}
	} else {
		pawns[col].setBit(SQ64(sq));
		pawns[BOTH].setBit(SQ64(sq));
	}
	material[col] += getPieceVal(pce);
	pList[pce][pceNum[pce]++] = sq;
}

void Board::movePiece(
	const int32_t from,
	const int32_t to
) {
	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	int32_t pce = pieces[from];
	int32_t col = getPieceCol(pce);

#ifdef DEBUG
	int32_t t_PieceNum = false;
#endif

	hashPce(pce, from);
	pieces[from] = EMPTY;

	hashPce(pce, to);
	pieces[to] = pce;

	if(!isPieceBig(pce)) {
		pawns[col].clearBit(SQ64(from));
		pawns[BOTH].clearBit(SQ64(from));
		pawns[col].setBit(SQ64(to));
		pawns[BOTH].setBit(SQ64(to));
	}

	for(int32_t i = 0; i < pceNum[pce]; ++i) {
		if(pList[pce][i] == from) {
			pList[pce][i] = to;
#ifdef DEBUG
			t_PieceNum = true;
#endif
			break;
		}
	}

	ASSERT(t_PieceNum);
}

// [ Attack Methods ]

const bool Board::sqAttacked(
	const int32_t sq,
	const Alliance side
) {
	static const int32_t KnDir[8] = {
		-8, -19, -21, -12, 8, 19, 21, 12
	};
	static const int32_t RkDir[4] = {
		-1, -10, 1, 10 
	};
	static const int32_t BiDir[4] = { 
		-9, -11, 11, 9
	};
	static const int32_t KiDir[8] = {
		-1, -10, 1, 10, -9, -11, 11, 9
	};

	int32_t pce, index, t_sq, dir;
	
	ASSERT(isSqOnBoard(sq));
	ASSERT(isSideValid(side));
	ASSERT(this->checkBoard());
	
	// pawns
	if(side == WHITE) {
		if(pieces[sq-11] == wP || pieces[sq-9] == wP) {
			return true;
		}
	} else {
		if(pieces[sq+11] == bP || pieces[sq+9] == bP) {
			return true;
		}	
	}
	
	// knights
	for(index = 0; index < 8; ++index) {		
		pce = pieces[sq + KnDir[index]];
		if(pce != OFFBOARD && IsKn(pce) &&getPieceCol[pce]==side) {
			return true;
		}
	}
	
	// rooks, queens
	for(index = 0; index < 4; ++index) {		
		dir = RkDir[index];
		t_sq = sq + dir;
		pce = pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if(IsRQ(pce) &&getPieceCol[pce] == side) {
					return true;
				}
				break;
			}
			t_sq += dir;
			pce = pieces[t_sq];
		}
	}
	
	// bishops, queens
	for(index = 0; index < 4; ++index) {		
		dir = BiDir[index];
		t_sq = sq + dir;
		pce = pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if(IsBQ(pce) &&getPieceCol[pce] == side) {
					return true;
				}
				break;
			}
			t_sq += dir;
			pce = pieces[t_sq];
		}
	}
	
	// kings
	for(index = 0; index < 8; ++index) {		
		pce = pieces[sq + KiDir[index]];
		if(pce != OFFBOARD && IsKi(pce) &&getPieceCol[pce]==side) {
			return true;
		}
	}
	
	return false;
}

// [ Evaluate Methods ]

const int32_t Board::eval() {
	return Evaluate::evaluatePosition(*this);
}

// [ Move Methods ]

const bool Board::doesMoveExist(const int32_t move) {
	MoveList list;
	list.generateFrom(*this);
      
    int32_t moveNum = 0;
	for(moveNum = 0; moveNum < list.count; ++moveNum) {	
        if (!makeMove(list.moves[moveNum].move))  {
            continue;
        }        
        takeMove();
		if(list.moves[moveNum].move == move) {
			return true;
		}
    }
	return false;
}

const int32_t Board::parseMove(const string mv) {
	if(mv[0] < 'a' || mv[0] > 'h') return NOMOVE;
	if(mv[2] < 'a' || mv[2] > 'h') return NOMOVE;
	if(mv[1] < '0' || mv[1] > '8') return NOMOVE;
	if(mv[3] < '0' || mv[3] > '8') return NOMOVE;

	int32_t from = FR2SQ(mv[0] - 'a', mv[1] - '1');
	int32_t to = FR2SQ(mv[2] - 'a', mv[3] - '1');
	ASSERT(isSqOnBoard(from) && isSqOnBoard(to));

	MoveList ml(*this);
	int32_t promPce;

	for(int32_t moveNum = 0; moveNum < ml.count; ++moveNum) {
		Move& move = ml.moves[moveNum];
		if(move.getFrom() == from && move.getTo() == to) {
			promPce = move.getPromoted();
			if(promPce != EMPTY) {
				if(IsRQ(promPce) && !IsBQ(promPce) && mv[4] == 'r')
					return move.move;
				else if(!IsRQ(promPce) && IsBQ(promPce) && mv[4] == 'b')
					return move.move;
				else if(IsRQ(promPce) && IsBQ(promPce) && mv[4] == 'q')
					return move.move;
				else if(IsKn(promPce) && mv[4] == 'n')
					return move.move;
				continue;
			}
			return move.move;
		}
	}

	return NOMOVE;
}

void Board::takeMove() {
	ASSERT(checkBoard());
	--hisPly;
	--ply;

	Move move(history[hisPly].move);
	int32_t from = move.getFrom();
	int32_t to = move.getTo();

	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));

	if(enPas != NO_SQ)
		hashEP();
	hashCA();

	castlePerm = history[hisPly].castlePerm;
	fiftyMove = history[hisPly].fiftyMove;
	enPas = history[hisPly].enPas;

	if(enPas != NO_SQ)
		hashEP();
	hashCA();

	side = (side == WHITE) ? BLACK : WHITE;
	hashSide();

	if(move.getEP()) {
		if(side == WHITE)
			addPiece(to-10, bP);
		else
			addPiece(to+10, wP);
	} else if(move.getCA()) {
		switch(to) {
			case C1:
				movePiece(D1, A1);
				break;
			case C8:
				movePiece(D8, A8);
				break;
			case G1:
				movePiece(F1, H1);
				break;
			case G8:
				movePiece(F8, H8);
				break;
			default:
				ASSERT(false);
				break;
		}
	}

	movePiece(to, from);

	if(isPieceKing(pieces[from])) {
		KingSq[side] = from;
	}

	int32_t captured = move.getCaptured();
	if(captured != EMPTY) {
		ASSERT(isPieceValid(captured));
		addPiece(to, captured);
	}

	if(move.getPromoted() != EMPTY) {
		ASSERT(isPieceValid(move.getPromoted()));
		ASSERT(!isPiecePawn(move.getPromoted()));
		clearPiece(from);
		addPiece(
			from,
			((getPieceCol(move.getPromoted()) == WHITE) ? wP : bP)
		);
	}
	ASSERT(checkBoard());
}

const bool Board::makeMove(const int32_t move) {
	return makeMove(Move(move));
}

const bool Board::makeMove(Move move) {
	ASSERT(checkBoard());

	int32_t from = move.getFrom();
	int32_t to = move.getTo();

	ASSERT(isSqOnBoard(from));
	ASSERT(isSqOnBoard(to));
	ASSERT(isSideValid(side));
	ASSERT(isPieceValid(pieces[from]));

	history[hisPly].posKey = posKey;
	if(move.getEP()) {
		if(side == WHITE)
			clearPiece(to-10);
		else
			clearPiece(to+10);
	} else if(move.getCA()) {
		switch(to) {
			case C1:
				movePiece(A1, D1);
				break;
			case C8:
				movePiece(A8, D8);
				break;
			case G1:
				movePiece(H1, F1);
				break;
			case G8:
				movePiece(H8, F8);
				break;
			default:
				ASSERT(false);
				break;
		}
	}

	if(enPas != NO_SQ) hashEP();
	hashCA();

	history[hisPly].move = move.move;
	history[hisPly].fiftyMove = fiftyMove;
	history[hisPly].enPas = enPas;
	history[hisPly].castlePerm = castlePerm;

	castlePerm &= CastlePerm[from];
	castlePerm &= CastlePerm[to];
	enPas = NO_SQ;
	hashCA();

	int32_t captured = move.getCaptured();
	++fiftyMove;

	if(captured != EMPTY) {
		ASSERT(isPieceValid(captured));
		clearPiece(to);
		fiftyMove = 0;
	}

	++hisPly;
	++ply;

	if(isPiecePawn(pieces[from])) {
		fiftyMove = 0;
		if(move.getPS()) {
			if(side == WHITE) {
				enPas = from + 10;
				ASSERT(RanksBrd[enPas] == RANK_3);
			} else {
				enPas = from - 10;
				ASSERT(RanksBrd[enPas] == RANK_6);
			}
			hashEP();
		}
	}

	movePiece(from, to);

	int32_t prPce = move.getPromoted();
	if(prPce != EMPTY) {
		ASSERT(isPieceValid(prPce) && !isPiecePawn(prPce));
		clearPiece(to);
		addPiece(to, prPce);
	}

	if(isPieceKing(pieces[to])) {
		KingSq[side] = to;
	}

	Alliance oldSide = side;
	side = (side == WHITE) ? BLACK : WHITE;
	hashSide();

	ASSERT(checkBoard());
	if(sqAttacked(KingSq[oldSide], side)) {
		takeMove();
		return false;
	}

	return true;
}

void Board::makeNullMove() {
	ASSERT(checkBoard());

	ply++;
	history[hisPly].posKey = posKey;

	if(enPas != NO_SQ)
		hashEP();

	history[hisPly].move = NOMOVE;
	history[hisPly].fiftyMove = fiftyMove;
	history[hisPly].enPas = enPas;
	history[hisPly].castlePerm = castlePerm;
	enPas = NO_SQ;

	side = (side == WHITE) ? BLACK : WHITE;
	hisPly++;
	hashSide();
	ASSERT(checkBoard());
}

void Board::takeNullMove() {
	ASSERT(checkBoard());

	hisPly--;
	ply--;

	if(enPas != NO_SQ) hashEP();

	castlePerm = history[hisPly].castlePerm;
	fiftyMove = history[hisPly].fiftyMove;
	enPas = history[hisPly].enPas;

	if(enPas != NO_SQ) hashEP();
	side = (side == WHITE) ? BLACK : WHITE;
	hashSide();
	ASSERT(checkBoard());
}

// [ Game Management Methods ]

const int32_t Board::threeFoldRep() {
	int32_t r = 0;
	for(int32_t i = 0; i < hisPly; ++i) {
		if(history[i].posKey == posKey) {
			++r;
		}
	}
	return r;
}

const bool Board::drawMaterial() {
	if (!pceNum[wR] && !pceNum[bR] && !pceNum[wQ] && !pceNum[bQ]) {
	  	if (!pceNum[bB] && !pceNum[wB]) {
	      	if (pceNum[wN] < 3 && pceNum[bN] < 3) {  return true; }
	  	} else if (!pceNum[wN] && !pceNum[bN]) {
	     	if (abs(pceNum[wB] - pceNum[bB]) < 2) { return true; }
	  	} else if ((pceNum[wN] < 3 && !pceNum[wB]) || (pceNum[wB] == 1 && !pceNum[wN])) {
	    	if ((pceNum[bN] < 3 && !pceNum[bB]) || (pceNum[bB] == 1 && !pceNum[bN]))  { return true; }
	  	}
	} else if (!pceNum[wQ] && !pceNum[bQ]) {
        if (pceNum[wR] == 1 && pceNum[bR] == 1) {
            if ((pceNum[wN] + pceNum[wB]) < 2 && (pceNum[bN] + pceNum[bB]) < 2)	{ return true; }
        } else if (pceNum[wR] == 1 && !pceNum[bR]) {
            if ((pceNum[wN] + pceNum[wB] == 0) && (((pceNum[bN] + pceNum[bB]) == 1) || ((pceNum[bN] + pceNum[bB]) == 2))) { return true; }
        } else if (pceNum[bR] == 1 && !pceNum[wR]) {
            if ((pceNum[bN] + pceNum[bB] == 0) && (((pceNum[wN] + pceNum[wB]) == 1) || ((pceNum[wN] + pceNum[wB]) == 2))) { return true; }
        }
    }
  	return false;
}

const bool Board::checkResult() {
	if(fiftyMove > 100) {
		printf("1/2-1/2 {fifty move rule (claimed by WiCE)}\n");
		return true;
	}
	if(threeFoldRep() >= 2) {
		printf("1/2-1/2 {3-fold repetition (claimed by WiCE)}\n");
		return true;
	}
	if(drawMaterial()) {
		printf("1/2-1/2 {insufficient material (claimed by WiCE)}\n");
		return true;
	}

	bool found = false;
	MoveList ml(*this);
	for(int32_t i = 0; i < ml.count; ++i) {
		if(!makeMove(ml.moves[i]))
			continue;
		found = true;
		takeMove();
		break;
	}

	if(found == true) return false;
	bool inCheck = sqAttacked(
		KingSq[side],
		(side == WHITE) ? BLACK : WHITE
	);

	if(inCheck) {
		if(side == WHITE) {
			printf("0-1 {black mates (claimed by WiCE)}\n");
			return true;
		} else {
			printf("0-1 {white mates (claimed by WiCE)}\n");
			return true;
		}
	} else {
		printf("1/2-1/2 {stalemate (claimed by WiCE)}\n");
		return false;
	}
}

// [ PV Methods ]

const int32_t Board::probePVTable() {
	int32_t i = posKey % (hashTable->numEntries);
	ASSERT(i >= 0 && i <= hashTable->numEntries - 1);
	
	if(hashTable->pTable[i].posKey == posKey) {
		return hashTable->pTable[i].move;
	}
	return NOMOVE;
}

const int32_t Board::getPVLine(const int32_t depth) {
	ASSERT(depth < MAX_DEPTH);

	int32_t move = probePVTable();
	int32_t count = 0;

	while(move != NOMOVE && count < depth) {
		ASSERT(count < MAX_DEPTH);
		if(doesMoveExist(move)) {
			makeMove(move);
			pvArray[count++] = move;
		} else {
			break;
		}
		move = probePVTable();
	}

	while(ply > 0) {
		takeMove();
	}

	return count;
}

void Board::storeHashEntry(
	const int32_t move,
	int32_t score,
	const int32_t flags,
	const int32_t depth
) {
	int32_t i = posKey % hashTable->numEntries;
	ASSERT(i >= 0 && i <= hashTable->numEntries - 1);
	ASSERT(depth >= 1 && depth < MAX_DEPTH);
	ASSERT(flags >= HFNONE && flags <= HFEXACT);
	ASSERT(score >= -WiCE_INF && score <= WiCE_INF);
	ASSERT(ply >= 0 && ply < MAX_DEPTH);

	if(hashTable->pTable[i].posKey == 0) {
		hashTable->newWrite++;
	} else {
		hashTable->overWrite++;
	}

	if(score > ISMATE) score += ply;
	else if(score < -ISMATE) score -= ply;

	hashTable->pTable[i].move = move;
	hashTable->pTable[i].posKey = posKey;
	hashTable->pTable[i].flags = flags;
	hashTable->pTable[i].score = score;
	hashTable->pTable[i].depth = depth;
}

const bool Board::probeHashEntry(
	int32_t& move,
	int32_t& score,
	const int32_t alpha,
	const int32_t beta,
	const int32_t depth
) {
	int32_t i = posKey % hashTable->numEntries;
	ASSERT(i >= 0 && i <= hashTable->numEntries - 1);
	ASSERT(depth >= 1 && depth < MAX_DEPTH);
	ASSERT(alpha < beta);
	ASSERT(alpha >= -WiCE_INF && alpha <= WiCE_INF);
	ASSERT(beta >= -WiCE_INF && beta <= WiCE_INF);
	ASSERT(ply >= 0 && ply < MAX_DEPTH);

	if(hashTable->pTable[i].posKey == posKey) {
		move = hashTable->pTable[i].move;
		if(hashTable->pTable[i].depth >= depth) {
			hashTable->hit++;

			score = hashTable->pTable[i].score;
			if(score > ISMATE) score -= ply;
			else if(score < -ISMATE) score += ply;

			switch(hashTable->pTable[i].flags) {
				ASSERT(score >= -WiCE_INF && score <= WiCE_INF);
				case HFALPHA:
					if(score <= alpha) {
						score = alpha;
						return true;
					}
					break;
				case HFBETA:
					if(score >= beta) {
						score = beta;
						return true;
					}
					break;
				case HFEXACT:
					return true;
					break;
				default:
					ASSERT(false);
					break;
			}
		}
	}
	return false;
}

// [ Searching Methods ]

const bool Board::isRepetition() {
	for(int32_t i = hisPly - fiftyMove; i < hisPly - 1; ++i) {	
		ASSERT(i >= 0 && i < MAX_GAME_MOVES);		
		if(posKey == history[i].posKey) {
			return true;
		}
	}	
	return false;
}

void Board::clearForSearch() {
	int32_t i, j;
	for(i = 0; i < 13; ++i) {
		for(j = 0; j < 120; ++j) {
			searchHistory[i][j] = 0;
		}
	}
	for(i = 0; i < 2; ++i) {
		for(j = 0; j < MAX_DEPTH; ++j) {
			searchKillers[i][j] = 0;
		}
	}
	hashTable->overWrite = 0;
	hashTable->hit = 0;
	hashTable->cut = 0;
	ply = 0;
}

// [ Poly Methods ]

const uint64_t Board::getPolyKey() {
	int32_t sq = 0, rank = 0, file = 0;
	int32_t piece = EMPTY, polyPiece = 0;
	uint64_t finalKey = 0;

	// Pieces
	for(sq = 0; sq < 120; ++sq) {
		piece = pieces[sq];
		if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
			ASSERT(piece >= wP && piece <= bK);
			polyPiece = Poly::kindOfPiece[piece];
			rank = RanksBrd[sq];
			file = FilesBrd[sq];
			finalKey ^= Poly::random64[64 * polyPiece + 8 * rank + file];
		}
	}

	// Castling
	if(castlePerm & WKCA)
		finalKey ^= Poly::random64[768];
	if(castlePerm & WQCA)
		finalKey ^= Poly::random64[769];
	if(castlePerm & BKCA)
		finalKey ^= Poly::random64[770];
	if(castlePerm & BQCA)
		finalKey ^= Poly::random64[771];

	// Enpassant
	if(hasPawnForCapture()) {
		file = FilesBrd[enPas];
		finalKey ^= Poly::random64[772 + file];
	}

	// Side
	if(side == WHITE)
		finalKey ^= Poly::random64[780];

	return finalKey;
}

const bool Board::hasPawnForCapture() {
	int32_t sqWithPawn = 0;
	int32_t targetPce = (side == WHITE) ? wP : bP;
	if(enPas != NO_SQ) {
		if(side == WHITE) {
			sqWithPawn = enPas - 10;
		} else {
			sqWithPawn = enPas + 10;
		}

		if(pieces[sqWithPawn + 1] == targetPce) {
			return true;
		} else if(pieces[sqWithPawn - 1] == targetPce) {
			return true;
		}
	}
	return false;
}

// [ Output Methods ]

const string Board::toString() {
	int32_t sq, pt;
	string result = "Game Board:\n";

	for(int32_t r = RANK_8; r >= RANK_1; --r) {
		result += to_string(r+1) + "|  ";
		for(int32_t f = FILE_A; f <= FILE_H; ++f) {
			sq = FR2SQ(f, r);
			pt = pieces[sq];
			result.push_back(PceChar[pt]);
			result += "  ";
		}
		result.push_back('\n');
	}
	result += "    ______________________\n";
	result += "    A  B  C  D  E  F  G  H\n";
	result += "\nSide: ";
	result.push_back(SideChar[side]);
	result += "\nEnPas: " + to_string(enPas);
	result += "\nCastle: " + to_string(castlePerm);
	result += "\nPosKey: " + to_string(posKey);
	return result;
}

void Board::print() {
	printf("\n%s", toString().c_str());
}