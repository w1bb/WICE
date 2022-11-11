// ---------------------------------------------
// |      WiCE / dependencies / move.hpp       |
// ---------------------------------------------
// 
// Move and MoveList are defined in here. They
// are responsible to a lot of chaos around
// here... I tried mixing integers with structs
// and it is quite a mess...
// 
// ---------------------------------------------



#ifndef WiCE_Move_HPP
#define WiCE_Move_HPP "5d"

struct Move;
struct MoveList;

#include "utilities.hpp"
#include "board.hpp"
#include "data.hpp"

// ---------------------------------------------
// Move
// ---------------------------------------------

struct Move {
	// [ Variables / Constants ]
	static const int32_t victimScore[13];
	static const int32_t mvvLvaScores[13][13];

	static const int32_t EP_Flag;
	static const int32_t PS_Flag;
	static const int32_t CA_Flag;

	int32_t move;
	int32_t score;

	// [ Methods ]
	Move();
	Move(const int32_t _move);

	const int32_t getFrom();
	const int32_t getTo();
	const int32_t getCaptured();
	const int32_t getPromoted();
	const int32_t getEP();
	const int32_t getPS();
	const int32_t getCA();

	const bool wasCaptured();
	const bool wasPromoted();

	const string toString();
	void print();
};

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

// ---------------------------------------------
// MoveList
// ---------------------------------------------

struct MoveList {
	// [ Variables / Constants ]
	static const int32_t LoopSlidePce[8];
	static const int32_t LoopSlideIndex[2];
	static const int32_t LoopNonSlidePce[6];
	static const int32_t LoopNonSlideIndex[2];
	static const int32_t PceDir[13][8];
	static const int32_t NumDir[13];

	Move moves[MAX_POSITION_MOVES];
	int32_t count;

	// [ Methods ]
	MoveList();
	MoveList(Board& board);

	void pickNextMove(const int32_t moveNum);
	
	void addQuietMove(
		Board& pos,
		const int32_t move
	);
	void addCaptureMove(
		Board& pos,
		const int32_t move
	);
	void addEnPassantMove(
		Board& pos,
		const int32_t move
	);
	void addWhitePawnCapMove(
		Board& pos,
		const int32_t from,
		const int32_t to,
		const int32_t cap
	);
	void addWhitePawnMove(
		Board& pos,
		const int32_t from,
		const int32_t to
	);
	void addBlackPawnCapMove(
		Board& pos,
		const int32_t from,
		const int32_t to,
		const int32_t cap
	);
	void addBlackPawnMove(
		Board& pos,
		const int32_t from,
		const int32_t to
	);

	void generateFrom(Board& pos);
	void generateCapsFrom(Board& pos);

	const string toString();
	void print();
};

#endif // WiCE_PVT_HPP