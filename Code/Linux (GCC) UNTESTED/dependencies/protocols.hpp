// ---------------------------------------------
// |    WiCE / dependencies / protocols.hpp    |
// ---------------------------------------------
// 
// This library ensures correct communication
// between the engine and a GUI or console.
// 
// ---------------------------------------------



#ifndef WiCE_Protocols_HPP
#define WiCE_Protocols_HPP "5d"

struct UCI;
struct Console;

#include "utilities.hpp"
#include "search.hpp"
#include "board.hpp"

// ---------------------------------------------
// UCI
// ---------------------------------------------

struct UCI {
	// [ Methods ]
	UCI(); // not allowed!

	static const int32_t inputWaiting();
	static void readInput(SearchInfo& info);

	static void parseGo(
		char * line,
		SearchInfo& info,
		Board& pos,
		WiCEOptions& options,
		PolyBook& polyBook
	);
	static void parsePosition(
		char * lineIn,
		Board& pos
	);
	static void loop(
		WiCEOptions& options,
		PolyBook& polyBook
	);
};

// ---------------------------------------------
// Console
// ---------------------------------------------

struct Console {
	// [ Variables ]
	static Board board;

	// [ Methods ]
	Console(); // not allowed!

	static void loop(
		WiCEOptions& engineOptions,
		PolyBook& polyBook
	);
};

#endif // WiCE_Protocols_HPP