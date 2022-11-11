#include "evaluate.hpp"

// ---------------------------------------------
// Evaluate
// ---------------------------------------------

// [ Constants (to be init) ]

BitBoard Evaluate::WhitePassedMask[64];
BitBoard Evaluate::BlackPassedMask[64];
BitBoard Evaluate::IsolatedMask[64];

// [ Constants (known) ]

BitBoard Evaluate::FileBBMask[8] = {
	0x0101010101010101ull, 0x0202020202020202ull,
	0x0404040404040404ull, 0x0808080808080808ull,
	0x1010101010101010ull, 0x2020202020202020ull,
	0x4040404040404040ull, 0x8080808080808080ull
};

BitBoard Evaluate::RankBBMask[8] = {
	0x00000000000000ffull, 0x000000000000ff00ull,
	0x0000000000ff0000ull, 0x00000000ff000000ull,
	0x000000ff00000000ull, 0x0000ff0000000000ull,
	0x00ff000000000000ull, 0xff00000000000000ull
};

const int32_t Evaluate::pawnIsolated = -10;
const int32_t Evaluate::pawnPassed[8] = {
	0, 5, 10, 20, 35, 60, 100, 200
};
const int32_t Evaluate::rookOpenFile = 10;
const int32_t Evaluate::rookSemiOpenFile = 5;
const int32_t Evaluate::queenOpenFile = 5;
const int32_t Evaluate::queenSemiOpenFile = 3;

const int32_t Evaluate::KingE[64] = {	
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	0	,	10	,	15	,	15	,	15	,	15	,	10	,	0	,
	0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
	0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
	0	,	10	,	15	,	15	,	15	,	15	,	10	,	0	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	
};

const int32_t Evaluate::KingO[64] = {	
	0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
	-10	,	-10	,	-10	,	-10	,	-10	,	-10	,	-10	,	-10	,
	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70		
};

const int32_t Evaluate::bishopPair = 30;

// [ Methods ]

Evaluate::Evaluate() { // not allowed!
	throw Exception::E_Evaluate_Init_E;
}

// Evaluate::init()
// Initialize the remaining constants.
void Evaluate::init() {
	int32_t sq, tsq;
	
	for(sq = 0; sq < 64; ++sq) {
		IsolatedMask[sq] = 0ULL; 
		WhitePassedMask[sq] = 0ULL; 
		BlackPassedMask[sq] = 0ULL;
    }

	for(sq = 0; sq < 64; ++sq) {
		tsq = sq + 8;
        while(tsq < 64) {
            WhitePassedMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }
        tsq = sq - 8;
        while(tsq >= 0) {
            BlackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }
        if(FilesBrd[SQ120(sq)] > FILE_A) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] - 1];
            tsq = sq + 7;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
            tsq = sq - 9;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
        if(FilesBrd[SQ120(sq)] < FILE_H) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] + 1];
            tsq = sq + 9;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
            tsq = sq - 7;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
	}	
}

// Evaluate::evaluatePosition()
// Compute an integer value that will determine
// if the position is advantageous or not. It
// takes into consideration factors such as:
// * pieces on the board;
// * pieces position;
// * open files and ranks;
// * king safety in various moments of the game.
const int32_t Evaluate::evaluatePosition(Board& pos) {
	int32_t pce, i, sq;
	int32_t score = pos.material[WHITE] - pos.material[BLACK];
	
	if(!pos.pceNum[wP] && !pos.pceNum[bP] && pos.drawMaterial())
		return 0;

	pce = wP;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score += PawnTable[SQ64(sq)];

		if((IsolatedMask[SQ64(sq)] & pos.pawns[WHITE]) == 0) {
			score += pawnIsolated;
		} if((WhitePassedMask[SQ64(sq)] & pos.pawns[BLACK]) == 0) {
			score += pawnPassed[RanksBrd[sq]];
		}
	}

	pce = bP;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score -= PawnTable[Mirror64[SQ64(sq)]];

		if((IsolatedMask[SQ64(sq)] & pos.pawns[BLACK]) == 0) {
			score -= pawnIsolated;
		} if((BlackPassedMask[SQ64(sq)] & pos.pawns[WHITE]) == 0) {
			score -= pawnPassed[7 - RanksBrd[sq]];
		}
	}
	
	pce = wN;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score += KnightTable[SQ64(sq)];
	}

	pce = bN;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score -= KnightTable[Mirror64[SQ64(sq)]];
	}
	
	pce = wB;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score += BishopTable[SQ64(sq)];
	}

	pce = bB;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score -= BishopTable[Mirror64[SQ64(sq)]];
	}

	pce = wR;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score += RookTable[SQ64(sq)];

		if(!(pos.pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += rookOpenFile;
		} else if(!(pos.pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
			score += rookSemiOpenFile;
		}
	}

	pce = bR;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));
		score -= RookTable[Mirror64[SQ64(sq)]];

		if(!(pos.pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score -= rookOpenFile;
		} else if(!(pos.pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
			score -= rookSemiOpenFile;
		}
	}

	pce = wQ;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));

		if(!(pos.pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += queenOpenFile;
		} else if(!(pos.pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
			score += queenSemiOpenFile;
		}
	}

	pce = bQ;
	for(i = 0; i < pos.pceNum[pce]; ++i) {
		sq = pos.pList[pce][i];
		ASSERT(isSqOnBoard(sq));

		if(!(pos.pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score -= queenOpenFile;
		} else if(!(pos.pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
			score -= queenSemiOpenFile;
		}
	}

	pce = wK;
	sq = pos.pList[pce][0];
	if((pos.material[BLACK] <= ENDGAME_MAT))
		score += KingE[SQ64(sq)];
	else
		score += KingO[SQ64(sq)];

	pce = bK;
	sq = pos.pList[pce][0];
	if((pos.material[BLACK] <= ENDGAME_MAT))
		score -= KingE[SQ64(sq)];
	else
		score -= KingO[SQ64(sq)];
	
	if(pos.pceNum[wB] >= 2) score += bishopPair;
	if(pos.pceNum[bB] >= 2) score -= bishopPair;

	return (pos.side == WHITE) ? score : -score;
}