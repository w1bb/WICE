#include "protocols.hpp"

// ---------------------------------------------
// UCI
// ---------------------------------------------

// [ Methods ]

UCI::UCI() { // not allowed!
	throw Exception::E_UCI_Init_E;
}

// inputWaiting()
// A complex function that tests if there is
// some waiting input that needs to be parsed.
// This was inspired by another chess engine.
const int32_t UCI::inputWaiting()
{
#ifndef WiCE_Win
	fd_set readfds;
	struct timeval tv;
	FD_ZERO (&readfds);
	FD_SET (fileno(stdin), &readfds);
	tv.tv_sec=0; tv.tv_usec=0;
	select(16, &readfds, 0, 0, &tv);
	return (FD_ISSET(fileno(stdin), &readfds));
#else
   	static int32_t init = 0, pipe;
   	static HANDLE inh;
   	DWORD dw;
   	if (!init) {
   		init = 1;
     	inh = GetStdHandle(STD_INPUT_HANDLE);
     	pipe = !GetConsoleMode(inh, &dw);
     	if (!pipe) {
        	SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
        	FlushConsoleInputBuffer(inh) ;
      	}
    }
    if (pipe) {
      	if (!PeekNamedPipe(inh, nullptr, 0, nullptr, &dw, nullptr)) return 1;
      	return dw;
    } else {
      	GetNumberOfConsoleInputEvents(inh, &dw);
      	return (dw <= 1) ? 0 : dw;
	}
#endif
}

void UCI::readInput(SearchInfo& info) {
  	int32_t bytes;
  	char input[256] = "", *endc;

    if (inputWaiting()) {    
		info.stopped = true;
		do {
		 	bytes=_read(_fileno(stdin), input, 256);
		} while(bytes < 0);

		endc = strchr(input,'\n');
		if(endc) *endc = 0;

		if (strlen(input) > 0) {
			if (!strncmp(input, "quit", 4)) {
			  info.quit = true;
			}
		}
    }
}

// UCI::parseGo()
// Interprets the default UCI protocol call for 'go'.
void UCI::parseGo(
	char * line,
	SearchInfo& info,
	Board& pos,
	WiCEOptions& options,
	PolyBook& polyBook
) {
	int32_t depth = -1, movesToGo = 30, moveTime = -1;
	int32_t time = -1, inc = 0;
	char * ptr = nullptr;

	info.timeSet = false;

	if((ptr = strstr(line, "infinite"))) {}

	if((ptr = strstr(line, "binc")) && pos.side == BLACK)
		inc = atoi(ptr + 5);
	if((ptr = strstr(line, "winc")) && pos.side == WHITE)
		inc = atoi(ptr + 5);

	if((ptr = strstr(line, "btime")) && pos.side == BLACK)
		time = atoi(ptr + 6);
	if((ptr = strstr(line, "wtime")) && pos.side == WHITE)
		time = atoi(ptr + 6);

	if((ptr = strstr(line, "movestogo")))
		movesToGo = atoi(ptr + 10);

	if((ptr = strstr(line, "movetime")))
		moveTime = atoi(ptr + 9);

	if((ptr = strstr(line, "depth")))
		depth = atoi(ptr + 6);

	if(moveTime != -1) {
		time = moveTime;
		movesToGo = 1;
	}

	info.startTime = getTimeMs();
	info.depth = depth;

	if(time != -1) {
		info.timeSet = true;
		time /= movesToGo;
		time -= 20; // GUARANTEE
		info.stopTime = info.startTime + time + inc;
	}

	if(depth == -1) {
		info.depth = MAX_DEPTH;
	}

	printf("time:%d start:%" PRIu64 " stop:%" PRIu64 " depth:%d timeset:%d\n",
		time,
		info.startTime,
		info.stopTime,
		info.depth,
		info.timeSet
	);
	options.mode = MODE_UCI;
	Search::searchPosition(pos, info, options, polyBook);
}

// UCI::parsePosition()
// Protocol used by UCI to parse FENs.
void UCI::parsePosition(
	char * lineIn,
	Board& pos
) {
	lineIn += 9;
	char * ptrChar = lineIn;
	if(strncmp(lineIn, "startpos", 8) == 0) {
		pos.parseFEN(START_FEN);
	} else {
		ptrChar = strstr(lineIn, "fen");
		if(ptrChar == nullptr) {
			pos.parseFEN(START_FEN);
		} else {
			ptrChar += 4;
			pos.parseFEN(ptrChar);
		}
	}

	int32_t move;
	if((ptrChar = strstr(lineIn, "moves")) != nullptr) {
		ptrChar += 6;
		while(*ptrChar) {
			move = pos.parseMove(ptrChar);
			if(move == NOMOVE) break;
			pos.makeMove(move);
			pos.ply = 0;

			while(*ptrChar && *ptrChar != ' ')
				++ptrChar;
			++ptrChar;
		}
	}
	pos.print(); // testing purposes
	printf("\n\n");
}

// UCI::loop()
// Function used to parse everything that the
// UCI protocol is being fed.
void UCI::loop(
	WiCEOptions& options,
	PolyBook& polyBook
) {
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char line[INPUT_BUFFER];
	printf("id name %s\n", PROJECT_NAME);
	printf("id author Valentin-Ioan Vintila\n");
	printf("option name Hash type spin default 64 min 4 max %d\n", MAX_HASH);
	printf("option name Book type check default true\n");
	printf("uciok\n");

	Board pos;
	SearchInfo info;

	while(true) {
		memset(&line[0], 0, sizeof(line));
		fflush(stdout);
		if(!fgets(line, INPUT_BUFFER, stdin))
			continue;
		if(line[0] == '\n')
			continue;
		if(!strncmp(line, "isready", 7)) {
			printf("readyok\n");
			continue;
		} else if(!strncmp(line, "position", 8)) {
			parsePosition(line, pos);
		} else if(!strncmp(line, "ucinewgame", 10)) {
			parsePosition((char*)"position startpos\n", pos);
		} else if(!strncmp(line, "go", 2)) {
			parseGo(line, info, pos, options, polyBook);
		} else if(!strncmp(line, "quit", 4)) {
			info.quit = true;
			break;
		} else if(!strncmp(line, "uci", 3)) {
			printf("id name %s\n", PROJECT_NAME);
			printf("id author Valentin-Ioan Vintila\n");
			printf("uciok\n");
		} else if(!strncmp(line, "setoption name Hash value ", 26)) {
			int32_t MB;
			sscanf(line, "%*s %*s %*s %*s %d", &MB);
			if(MB < MIN_HASH) MB = MIN_HASH;
			else if(MB > MAX_HASH) MB = MAX_HASH;
			printf("Set Hash to %dMB\n", MB);
			pos.hashTable->reInit(MB);
		} else if(!strncmp(line, "setoption name Book value ", 26)) {
			char * ptrTrue = strstr(line, "true");
			if(ptrTrue != nullptr) {
				options.useBook = true;
			} else {
				options.useBook = false;
			}
		}
		if(info.quit) break;
	}
}

// ---------------------------------------------
// Console
// ---------------------------------------------

// [ Variables ]

Board Console::board;

// [ Methods ]

Console::Console() { // not allowed!
	throw Exception::E_Console_Init_E;
}

void Console::loop(
	WiCEOptions& engineOptions,
	PolyBook& polyBook
) {
	board.parseFEN(START_FEN);
	printf("[ WiCE 5a ]\n");
	printf("-----------\n");
	printf("Here is a short list of allowed commands:\n");
	printf("> [ l ]   Toggle the log\n");
	printf("> [ m ]   Change the memory size\n");
	printf("> [ p ]   Output polykey\n");
	printf("> [ q ]   Quit\n");
	printf("> [ s ]   Search at the currently set depth\n");
	printf("> [ t ]   Takeback a move\n");
	printf("> [ u ]   Update the depth (e.g.: u 12)\n");
	printf("> other   Will be considered a move (e.g.: a2a4)\n\n");
		
	int32_t atDepth = 7;
	bool redraw = true;
	char s[128];
	SearchInfo info;
	while(true) {
		if(redraw) {
			board.print();
			redraw = false;
		}
		printf("\nWiCE > ");
		scanf("%s", s);

		if(s[0] == 'l') {
			info.postThinking = info.postThinking ? false : true;
			if(info.postThinking) printf("The log is now ON\n");
			else printf("The log is now OFF\n");
		} else if(s[0] == 'm') {
			int32_t MB;
			sscanf(s, "m %d", &MB);
			if(MB < 4) MB = 4;
			else if(MB > 1024) MB = 1024;
			printf("Set Hash to %dMB\n", MB);
			board.hashTable->reInit(MB);
		} else if(s[0] == 'p') {
			printf("PolyKey: %lld\n", board.getPolyKey());
		} else if(s[0] == 'q') break;
		else if(s[0] == 's') {
			info.depth = atDepth;
			info.startTime = getTimeMs();
			info.stopTime = getTimeMs() + 200000;
			Search::searchPosition(
				board,
				info,
				engineOptions,
				polyBook
			);
		} else if(s[0] == 't') board.takeMove();
		else if(s[0] == 'u') {
			scanf("%d", &atDepth);
			if(atDepth <= 0)
				atDepth = 1;
			else if(atDepth > MAX_DEPTH)
				atDepth = MAX_DEPTH;
			printf("Depth successfuly modified: $atDepth=%d\n", atDepth);
		} else {
			int32_t mv = board.parseMove(s);
			if(mv != NOMOVE) {
				board.makeMove(mv);
				board.ply = 0;
				if(board.isRepetition() && info.postThinking) {
					printf("Repetition registered\n");
				}
				redraw = true;
			} else {
				printf("Invalid move!\n");
			}
		}
	}
}