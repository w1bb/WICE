// ---------------------------------------------
// |    WiCE / dependencies / utilities.hpp    |
// ---------------------------------------------
// 
// This library provides the basic functionality
// needed for the program to run, including STL
// data types, functions etc.
// 
// This library is a MUST in any of the other
// libraries used by WiCE.
// 
// ---------------------------------------------



#ifndef WiCE_Utilities_HPP
#define WiCE_Utilities_HPP "5d"

// ---------------------------------------------
// Warning Pragmas
// ===============
// Disable obstructing warnings so that we can
// focus on the real problems.
// ---------------------------------------------

#ifndef WiCE_Win
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

// ---------------------------------------------
// Project Metadata
// ================
// A name, a version and an author.
// ---------------------------------------------

#ifndef PROJECT_NAME
#define PROJECT_NAME    "Wi's Chess Engine (5a)"
#define PROJECT_VERSION "5a"
#define PROJECT_AUTHOR  "Valentin-Ioan Vintila"
#endif

// ---------------------------------------------
// Includes
// ========
// Include every STL or otherwise that will be
// used in this project.
// ---------------------------------------------

#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#define WiCE_Win
#else
#define WiCE_Lin
#endif

#ifdef WiCE_Win
#include <windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

// ---------------------------------------------
// Data Types
// ==========
// Only use this set of data types along the
// project (STL & otherwise).
// ---------------------------------------------

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::string;

enum WiCE_Mode {
	MODE_Console,
	MODE_Perft,
	MODE_UCI,
	MODE_xboard // huh, would you look at that?
	// an unimplemented (yet useless) feature!
};

enum PieceType {
	EMPTY,
	wP, wN, wB, wR, wQ, wK,
	bP, bN, bB, bR, bQ, bK
};

enum FileID {
	FILE_A, FILE_B, FILE_C, FILE_D,
	FILE_E, FILE_F, FILE_G, FILE_H,
	FILE_NONE
};

enum RankID {
	RANK_1, RANK_2, RANK_3, RANK_4,
	RANK_5, RANK_6, RANK_7, RANK_8,
	RANK_NONE
};

enum Alliance {
	WHITE, BLACK, BOTH
};

enum SquareID {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8,
	NO_SQ, OFFBOARD
};

enum CastleID {
	WKCA = 1, WQCA = 2,
	BKCA = 4, BQCA = 8
};

enum {
	HFNONE, HFALPHA, HFBETA, HFEXACT
};

// ---------------------------------------------
// Necesary Structures
// ===================
// These are some of the structures that could
// not fit in anywhere else.
// ---------------------------------------------

struct WiCEOptions {
	bool useBook = true;
	WiCE_Mode mode = MODE_UCI;
};

struct Undo {
	int32_t  move;
	int32_t  castlePerm;
	int32_t  enPas;
	int32_t  fiftyMove;
	uint64_t posKey;
};

// ---------------------------------------------
// Exceptions (non-std)
// ====================
// A different approach for handling exceptions.
// ---------------------------------------------

enum class Exception {
	E_UNDEFINED,
	E_UCI_Init_E,
	E_PolyBook_File_E,
	E_PolyBook_BadFile_E,
	E_Console_Init_E,
	E_Perft_Init_E,
	E_Perft_FIN_E,
	E_Search_Init_E,
	E_Evaluate_Init_E,
	E_Hash_Init_E,
	E_Board_FEN_E,
	E_HashTable_Init_Memory_E,
	E_PolyBook_ManyEntries,
	E_Unimplemented
};

static const string parseException(Exception e) {
	switch (e) {
	case Exception::E_UCI_Init_E:
		return "UCI::UCI() is not supposed to be called!";
	case Exception::E_PolyBook_File_E:
		return "PolyBook::PolyBook() could not load file!";
	case Exception::E_PolyBook_BadFile_E:
		return "PolyBook::PolyBook() does not contain entries!";
	case Exception::E_Console_Init_E:
		return "Console::Console() is not supposed to be called!";
	case Exception::E_Perft_Init_E:
		return "Perft::Perft() is not supposed to be called!";
	case Exception::E_Perft_FIN_E:
		return "Input file for Perft cannot be loaded.";
	case Exception::E_Search_Init_E:
		return "Search::Search() is not supposed to be called!";
	case Exception::E_Evaluate_Init_E:
		return "Evaluate::Evaluate() is not supposed to be called!";
	case Exception::E_Hash_Init_E:
		return "Hash::Hash() is not supposed to be called!";
	case Exception::E_Board_FEN_E:
		return "Board::parseFEN() could not parse a fen!";
	case Exception::E_HashTable_Init_Memory_E:
		return "HashTable::reInit() could not allocate enough memory!";
	case Exception::E_PolyBook_ManyEntries:
		return "Way too many entries in polybook!";
	case Exception::E_Unimplemented:
		return "A requested feature was not implemented yet!";
	case Exception::E_UNDEFINED:
	default:
		return "An undefined exception was thrown!";
	}
}

// ---------------------------------------------
// STL Functions
// =============
// Here are the required STL functions.
// ---------------------------------------------

using std::to_string;

// ---------------------------------------------
// Debug Macros
// ============
// A very useful debugging macro that allows for
// asserts everywhere without the computational
// additional complexity that comes with STL.
// Also, the WARNING macro.
// ---------------------------------------------

#ifndef DEBUG
	#define ASSERT(n)
#else
	#define ASSERT(n) \
	if(!(n)) { \
		printf("%s - Failed\n",  #n); \
		printf("On %s\n",       __DATE__); \
		printf("At %s\n",       __TIME__); \
		printf("In File %s",    __FILE__); \
		printf("- line %d\n",   __LINE__); \
		exit(1); \
	}
#endif

#ifdef WiCE_Win
#define WARNING(X) pragma message(X)
#else
#define WARNING(X) warning X
#endif

// ---------------------------------------------
// Other Functions
// ===============
// A collection of useful functions
// ---------------------------------------------

#ifdef _MSC_VER
#define U64(u) (u##ui64)
#else
#define U64(u) (u##ULL)
#endif

#define isSqOnBoard(sq)        (FilesBrd[sq] != OFFBOARD)
#define isSideValid(side)      (side == WHITE || side == BLACK)
#define isFileRankValid(fr)    (fr >= 0 && fr < 8)
#define isPieceValidEmpty(pce) (pce >= EMPTY && pce <= bK)
#define isPieceValid(pce)      (pce >= wP && pce <= bK)

#define FR2SQ(f,r)  (21 + (f) + (r) * 10) 
#define SQ64(sq120) Sq120ToSq64[(sq120)]
#define SQ120(sq64) Sq64ToSq120[(sq64)]

// MOVE()
// Create a move out of thin air (and variables)
#define MOVE(f, t, ca, pro, fl) \
	((f) | ((t) << 7) | \
	((ca) << 14) | ((pro) << 20) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

// getTimeMs()
// Compute the ammount of seconds since 1970.
static const uint64_t getTimeMs() { 
#ifdef WiCE_Win
  	return GetTickCount64();
#else
  	timeval t;
  	gettimeofday(&t, NULL);
  	return U64(t.tv_sec * 1000 + t.tv_usec / 1000);
#endif
}

static const uint16_t endianSwap16(const uint16_t x) {
	return ((x >> 8) | (x << 8));
}

static const uint32_t endianSwap32(const uint32_t x) {
	return ((x >> 24) | ((x << 8) & 0x00ff0000) |
		    ((x >> 8) & 0x0000ff00) | (x << 24));
}

static const uint64_t endianSwap64(const uint64_t x) {
	return ((x >> 56) |
		    ((x << 40) & U64(0x00ff000000000000)) |
		    ((x << 24) & U64(0x0000ff0000000000)) |
		    ((x << 8)  & U64(0x000000ff00000000)) |
		    ((x >> 8)  & U64(0x00000000ff000000)) |
		    ((x >> 24) & U64(0x0000000000ff0000)) |
		    ((x >> 40) & U64(0x000000000000ff00)) |
		    (x << 56));
}

// ENDGAME_MAT
// Is it the endgame yet?
#define ENDGAME_MAT \
(getPieceVal[wR] + 2 * getPieceVal[wN] + 2 * getPieceVal[wP])

// ---------------------------------------------
// Legacy Functions
// ================
// These were at some point actual functions.
// However, now they are simply lookup tables.
// For compatibility reasons, there will be a
// (macro) function version for each table.
// ---------------------------------------------

#define isPieceKing(x) isPieceKing[x]
#define isPiecePawn(x) isPiecePawn[x]
#define getPieceCol(x) getPieceCol[x]
#define getPieceVal(x) getPieceVal[x]
#define isPieceBig(x)  isPieceBig[x]
#define isPieceMaj(x)  isPieceMaj[x]
#define isPieceMin(x)  isPieceMin[x]

// These are also shorts for:
#define IsBQ(p) isPieceBQ[p]
#define IsRQ(p) isPieceRQ[p]
#define IsKn(p) isPieceKnight[p]
#define IsKi(p) isPieceKing[p]



#endif // WiCE_Utilities_HPP