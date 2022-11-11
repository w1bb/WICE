// ---------------------------------------------
// |     WiCE / dependencies / search.hpp      |
// ---------------------------------------------
// 
// This library creates the means to search for
// the best moves.
// 
// ---------------------------------------------



#ifndef WiCE_Search_HPP
#define WiCE_Search_HPP "5d"

struct SearchInfo;
struct Search;

#include "protocols.hpp"
#include "board.hpp"

// ---------------------------------------------
// SearchInfo
// ---------------------------------------------

struct SearchInfo {
	// [ Variables ]
	uint64_t startTime, stopTime;
	int32_t depth, depthSet;
	bool timeSet;

	int32_t movesToGo;
	int64_t nodes;
	bool infinite;

	bool quit, stopped;
	int32_t fh, fhf;

	bool postThinking;

	// [ Methods ]
	SearchInfo();
	void clearForSearch();
};

// ---------------------------------------------
// Search
// ---------------------------------------------

struct Search {
	// [ Variables ]
	const static int32_t checkUpNodes;

	// [ Methods ]
	Search(); // not allowed!

	static void prepare(
		Board& pos,
		SearchInfo& info
	);
	static void checkUp(SearchInfo& info);

	static void searchPosition(
		Board& pos,
		SearchInfo& info,
		WiCEOptions& options,
		PolyBook& polyBook
	);

	static const int32_t alphabeta(
		int32_t alpha,
		const int32_t beta,
		int32_t depth,
		Board& pos,
		SearchInfo& info,
		const bool doNull
	);
	static const int32_t quiescence(
		int32_t alpha,
		const int32_t beta,
		Board& pos,
		SearchInfo& info
	);
};


#endif // WiCE_Search_HPP