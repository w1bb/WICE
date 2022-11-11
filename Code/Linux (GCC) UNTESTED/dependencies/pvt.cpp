#include "pvt.hpp"

// ---------------------------------------------
// HashEntry
// ---------------------------------------------

HashEntry::HashEntry()
: posKey(0), move(0) {}

void HashEntry::clear() {
	posKey = 0;
	move = NOMOVE;
	depth = 0;
	score = 0;
	flags = 0;
}

// ---------------------------------------------
// HashTable
// ---------------------------------------------

// [ Variables / Constants ]

int32_t HashTable::pvSize = 0x100000 * 8;

// [ Methods ]

HashTable::HashTable()
: pTable(nullptr) {
	reInit(8);
}

HashTable::~HashTable() {
	if(pTable != nullptr) {
    	free(pTable);
    	pTable = nullptr;
	}
}

void HashTable::clear() {
	for (
		HashEntry * pvEntry = pTable;
		pvEntry < pTable + numEntries;
		++pvEntry
	) {
	    pvEntry->clear();
	}
	newWrite = 0;
}

void HashTable::reInit(int32_t MB) {
    if(pTable != nullptr) {
    	free(pTable);
    	pTable = nullptr;
    }
    while(MB > 0) {
    	pvSize = 0x100000 * MB;
    	numEntries = pvSize / sizeof(HashEntry) - 2;
    	pTable = (HashEntry*) malloc(
    		numEntries * sizeof(HashEntry)
    	);
    	if(pTable == nullptr)
    		MB >>= 1;
    	else break;
    }
    if(pTable == nullptr || !MB)
    	throw Exception::E_HashTable_Init_Memory_E;
    clear();
    printf("HashTable (re)init complete with %d entries.\n",numEntries);
}