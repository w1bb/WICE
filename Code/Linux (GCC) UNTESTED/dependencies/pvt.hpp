// ---------------------------------------------
// |       WiCE / dependencies / pvt.hpp       |
// ---------------------------------------------
// 
// PVT = Principal Variation Table
// It is very useful in searching, since it
// allows for good alpha beta pruning.
//
// However, the tech is outdated, even for our
// engine! PVT is no longer used, it has since
// been replaced by the Hash Table!
// 
// ---------------------------------------------



#ifndef WiCE_PVT_HPP
#define WiCE_PVT_HPP "5d"

struct HashEntry;
struct HashTable;

#include "data.hpp"

// ---------------------------------------------
// HashEntry
// ---------------------------------------------

struct HashEntry {
	// [ Variables ]
	uint64_t posKey;
	int32_t move;
	int32_t score;
	int32_t depth;
	int32_t flags;

	// [ Methods ]
	HashEntry();
	void clear();
};

// ---------------------------------------------
// HashTable
// ---------------------------------------------

struct HashTable {
	// [ Variables ]
	static int32_t pvSize;

	HashEntry * pTable;
	int32_t numEntries;
	int32_t newWrite;
	int32_t overWrite;
	int32_t hit;
	int32_t cut;

	// [ Methods ]
	HashTable();
	~HashTable();
	
	void reInit(const int32_t MB);
	void clear();
};

#endif // WiCE_PVT_HPP