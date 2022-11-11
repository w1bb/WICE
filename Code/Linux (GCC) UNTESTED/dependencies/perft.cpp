#include "perft.hpp"

// ----------------------------------------
// Perft
// ----------------------------------------

// [ Variables ]

Board Perft::b;
int32_t Perft::leafNodes;
int32_t Perft::auxgen;

// [ Methods ]

Perft::Perft() {
	throw Exception::E_Perft_Init_E;
}

const bool Perft::begin(
	const int32_t maxTests,
	const int32_t maxDepth
) {
	bool ok = true;

	printf("\nBeginning Perft Test\n");
	printf("--------------------\n\n");

	FILE * fin = fopen("WiCE/dependencies/perftsuite.epd", "r");
	if(!fin) throw Exception::E_Perft_FIN_E;

	char buffer[256];
	int32_t curTest = 1;

	while(fgets(buffer, 256, fin) && curTest <= maxTests) {
		char * token = strtok(buffer, ";");
		b.parseFEN(token);

		printf("[ %d ] FEN Loaded: %s\n",
			curTest, token);

		while((token = strtok(nullptr, ";"))) {
			int32_t rdepth = token[1] - '0';
			printf("| > Depth: %d", rdepth);
			if(rdepth <= maxDepth) {
				int32_t expectedVal = atoi(token + 3);
				printf("\n| > Expected: %d", expectedVal);

				leafNodes = auxgen = 0;
				const uint64_t startTime = getTimeMs();
				dfs(rdepth);
				const uint64_t endTime = getTimeMs();

				printf("\n| > WiCE result: %d (in %" PRIu64 "ms)",
					leafNodes,
					endTime - startTime
				);

				if(leafNodes != expectedVal) {
					printf("\n| > Problem encountered: NOT OK!");
					ok = false;
				}
			} else {
				printf(" (ignored, depth > %d)", maxDepth);
			}
		}

		printf("\n\n");
		++curTest;
	}

	fclose(fin);

	if(ok) {
		printf("---------------\n");
		printf("Conclusion: OK!\n\n");
	} else {
		printf("-------------------\n");
		printf("Conclusion: NOT OK!\n\n");
	}
	
	return ok;
}

void Perft::dfs(const int32_t depth) {
	ASSERT(b.checkBoard());
	if(depth == 0) {
		++leafNodes;
		return;
	}
	MoveList ml;
	ml.generateFrom(b);
	int32_t MoveNum = 0;
	for(MoveNum = 0; MoveNum < ml.count; ++MoveNum) {
		Move auxM;
		auxM.move = (ml.moves[(MoveNum)].move);
		if(!b.makeMove(ml.moves[(MoveNum)]))
			continue;
		dfs(depth - 1);
		b.takeMove();
	}
}