// ---------------------------------------------
// |      WiCE / dependencies / board.hpp      |
// ---------------------------------------------
// 
// Maybe the most important library. Everything
// that requires the chess board can be found
// in here - which is 99% of a chess game.
// 
// ---------------------------------------------



#ifndef WiCE_Board_HPP
#define WiCE_Board_HPP "5d"

struct Board;

#include "bitboard.hpp"
#include "evaluate.hpp"
#include "data.hpp"
#include "hash.hpp"
#include "move.hpp"
#include "poly.hpp"
#include "pvt.hpp"

// ---------------------------------------------
// Board
// ---------------------------------------------

struct Board {
	// [ Variables ]
	int32_t pieces[120];
	BitBoard pawns[3];

	int32_t KingSq[2];
	Alliance side;
	int32_t enPas;

	int32_t fiftyMove;

	int32_t ply;
	int32_t hisPly;
	uint64_t posKey;

	int32_t pceNum[13];
	int32_t bigPce[2];
	int32_t majPce[2];
	int32_t minPce[2];
	int32_t material[2];

	int32_t castlePerm;

	Undo history[MAX_GAME_MOVES];

	int32_t pList[13][10];

	HashTable hashTable[1];
	int32_t pvArray[MAX_DEPTH];
	int32_t searchHistory[13][120];
	int32_t searchKillers[2][MAX_DEPTH];

	// [ Loading Methods ]
	void parseFEN(char * fen);
	void mirrorBoard();

	// [ Maintenance Methods ]
	const bool checkBoard();
	void updateListMaterial();
	void reset();

	// [ Hashing Methods ]
	void hashPce(
		const int32_t pce,
		const int32_t sq
	);
	void hashCA();
	void hashSide();
	void hashEP();

	// [ Piece Methods ]
	void clearPiece(const int32_t sq);
	void addPiece(
		const int32_t sq,
		const int32_t pce
	);
	void movePiece(
		const int32_t from,
		const int32_t to
	);

	// [ Attack Methods ]
	const bool sqAttacked(
		const int32_t sq,
		const Alliance side
	);

	// [ Evaluate Methods ]
	const int32_t eval();

	// [ Move Methods ]
	const bool doesMoveExist(const int32_t move);
	const int32_t parseMove(const string mv);
	void takeMove();
	const bool makeMove(const int32_t move);
	const bool makeMove(Move move);
	void makeNullMove();
	void takeNullMove();

	// [ Game Management Methods ]
	const int32_t threeFoldRep();
	const bool drawMaterial();
	const bool checkResult();

	// [ PV Methods ]
	const int32_t probePVTable();
	const int32_t getPVLine(const int32_t depth);

	void storeHashEntry(
		const int32_t move,
		int32_t score,
		const int32_t flags,
		const int32_t depth
	);
	const bool probeHashEntry(
		int32_t& move,
		int32_t& score,
		const int32_t alpha,
		const int32_t beta,
		const int32_t depth
	);

	// [ Searching Methods ]
	const bool isRepetition();
	void clearForSearch();

	// [ Poly Methods ]
	const uint64_t getPolyKey();
	const bool hasPawnForCapture();

	// [ Output Methods ]
	const string toString();
	void print();
};

#endif // WiCE_Board_HPP