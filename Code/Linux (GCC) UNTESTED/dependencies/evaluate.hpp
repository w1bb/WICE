// ---------------------------------------------
// |    WiCE / dependencies / evaluate.hpp     |
// ---------------------------------------------
// 
// Evaluating the board is crucial. This is why,
// even though in other versions there was a
// Board::eval() method, we are now separating
// it in a dedicated header.
// 
// ---------------------------------------------



#ifndef WiCE_Evaluate_HPP
#define WiCE_Evaluate_HPP "5d"

struct Evaluate;

#include "board.hpp"

// ---------------------------------------------
// Evaluate
// ---------------------------------------------

struct Evaluate {
	// [ Constants (to be init) ]
	static BitBoard WhitePassedMask[64];
	static BitBoard BlackPassedMask[64];
	static BitBoard IsolatedMask[64];

	// [ Constants (known) ]
	static BitBoard FileBBMask[8];
	static BitBoard RankBBMask[8];

	static const int32_t pawnIsolated;
	static const int32_t pawnPassed[8];

	static const int32_t rookOpenFile;
	static const int32_t rookSemiOpenFile;
	static const int32_t queenOpenFile;
	static const int32_t queenSemiOpenFile;

	static const int32_t KingE[64];
	static const int32_t KingO[64];

	static const int32_t bishopPair;

	// [ Methods ]
	Evaluate(); // not allowed!
	static void init();
	static const int32_t evaluatePosition(Board& pos);
};

#endif // WiCE_Evaluate_HPP