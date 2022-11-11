#include "WiCE.hpp"

// ---------------------------------------------
// WiCE
// ---------------------------------------------

WiCE::WiCE(const WiCEOptions _engineOptions)
: engineOptions(_engineOptions),
  polyBook(engineOptions) {
	srand(NULL);
	Evaluate::init();
	if (engineOptions.mode == MODE_Console)
		Console::loop(engineOptions, polyBook);
	else if (engineOptions.mode == MODE_Perft)
		Perft::begin(1000, 5);
	else if (engineOptions.mode == MODE_UCI)
		UCI::loop(engineOptions, polyBook);
	else
		throw Exception::E_Unimplemented;
}

// ---------------------------------------------
// Main
// ---------------------------------------------

int main(int argc, char * argv[]) {
	WiCEOptions options;
	if (argc > 1) {
		if (strcmp(argv[1], "2") == 0)
			options.mode = MODE_Perft;
		else if (strcmp(argv[1], "3") == 0)
			options.mode = MODE_Console;
		else options.mode = MODE_UCI;
	} else options.mode = MODE_UCI;
#ifdef DEBUG
	printf("! DEBUG MODE ON !\nThe program might run slower than usual.\n\n");
#endif
	try {
		WiCE wice(options);
	} catch (Exception e) {
		printf("---\nAn exception was thrown!\n%s", parseException(e).c_str());
	}
	return 0;
}