// ---------------------------------------------
// |    WiCE / dependencies / bitboard.hpp     |
// ---------------------------------------------
// 
// An easy way to maipulate bits. It is kind of
// depreceated, since it is slower than normal
// bit manipulation.
// 
// ---------------------------------------------



#ifndef WiCE_BitBoard_HPP
#define WiCE_BitBoard_HPP "5d"

struct BitBoard;

#include "utilities.hpp"
#include "data.hpp"

// ---------------------------------------------
// BitBoard
// ---------------------------------------------

struct BitBoard {
	// [ Variables / Constants ]
	static const uint32_t BitTable[64];
	static const uint64_t SetMask[64];
	static const uint64_t ClearMask[64];
	uint64_t bb;

	// [ Methods ]
	BitBoard(const uint64_t _bb = 0);

	BitBoard& operator=(const uint64_t _bb);

	BitBoard& operator|=(const uint64_t ob);
	BitBoard& operator|=(BitBoard& ob);

	const uint64_t operator&(BitBoard& ob);

	BitBoard& clearBit(const int32_t sq);
	BitBoard& setBit(const int32_t sq);
	const int32_t popBit();
	const int32_t countBits();

	const uint64_t get();

	const string toString();
	void print();
};



#endif // WiCE_BitBoard_HPP