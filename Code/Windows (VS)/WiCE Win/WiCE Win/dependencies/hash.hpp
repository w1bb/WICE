// ---------------------------------------------
// |      WiCE / dependencies / hash.hpp       |
// ---------------------------------------------
// 
// This library provides basic hashing func-
// tionality. Many values have been precomputed.
// 
// ---------------------------------------------



#ifndef WiCE_Hash_HPP
#define WiCE_Hash_HPP "5d"

struct Hash;

#include "board.hpp"

// ---------------------------------------------
// Hash
// ---------------------------------------------

struct Hash {
	// [ Constants ]
	static const uint64_t PieceKeys[13][120];
	static const uint64_t SideKey;
	static const uint64_t CastleKeys[16];
	
	// [ Methods ]
	Hash();
	static const uint64_t generatePosKey(Board * pos);
};

#endif // WiCE_Hash_HPP