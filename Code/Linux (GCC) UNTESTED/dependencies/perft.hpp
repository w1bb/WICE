// ---------------------------------------------
// |      WiCE / dependencies / perft.hpp      |
// ---------------------------------------------
// 
// In order to make sure that the engine works,
// we need a way to test that we don't miss va-
// lid moves in our search. This is what the
// perft library is for.
// 
// ---------------------------------------------



#ifndef WiCE_Perft_HPP
#define WiCE_Perft_HPP "5d"

class Perft;

#include "utilities.hpp"
#include "board.hpp"
#include "move.hpp"

class Perft {
// [ Variables ]
private:
	static Board b;
	static int32_t leafNodes;
	static int32_t auxgen;

// [ Methods ]
public:
	Perft();
	static const bool begin(
		const int32_t maxTests,
		const int32_t maxDepth = 5
	);
private:
	static void dfs(const int32_t depth);
};

#endif // WiCE_Perft_HPP