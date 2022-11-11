// ---------------------------------------------
// |      WiCE / dependencies / poly.hpp       |
// ---------------------------------------------
// 
// Includes the required functionality for poly-
// glot (an older version of some sort).
// 
// ---------------------------------------------



#ifndef WiCE_Poly_HPP
#define WiCE_Poly_HPP "5d"

struct Poly;
struct PolyBookEntry;
struct PolyBook;

#include "board.hpp"

// ---------------------------------------------
// Poly
// ---------------------------------------------

struct Poly {
	// [ Constants ]
	static const int32_t kindOfPiece[13];
	static const uint64_t random64[781];

	// [ Methods ]
	static const int32_t convertPMtoIM(
		const uint16_t polyMove,
		Board& board
	);
};

// ---------------------------------------------
// PolyBookEntry
// ---------------------------------------------

struct PolyBookEntry {
	uint64_t key;
	uint16_t move;
	uint16_t weight;
	uint32_t learn;
};

// ---------------------------------------------
// PolyBook
// ---------------------------------------------

struct PolyBook {
	// [ Variables ]
	int64_t numEntries;
	PolyBookEntry * entries;

	// [ Methods ]
	PolyBook(WiCEOptions& engineOptions);
	void clear();

	const int32_t getBookMove(Board& board);

	
};

#endif // WiCE_Poly_HPP