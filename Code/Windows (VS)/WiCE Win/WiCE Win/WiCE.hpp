//     |#######################################|
//     |@                                     @|
//     |@        Wi's Chess Engine V5c        @|
//     |@        =====================        @|
//     |@                                     @|
//     |@  A quick and powerful chess engine  @|
//     |@  Designed by Valentin-Ioan Vintilă  @|
//     |@                                     @|
//     |#######################################|



//     License: GPLv3
//     --------------
//
//     Copyright 2021 Valentin-Ioan Vintilă.
//
//     A copy of the GPLv3 license was kindly
//     included in the projects' files.



//     Social
//     ------
//     Email: vintilavalentinioan@gmail.com



#ifndef WiCE_Core_HPP
#define WiCE_Core_HPP "5d"

#include "dependencies/protocols.hpp"
#include "dependencies/perft.hpp"

// ---------------------------------------------
// WiCE
// ---------------------------------------------

class WiCE {
// [ Variables ]
private:
	PolyBook polyBook;
	WiCEOptions engineOptions;

// [ Methods ]
public:
	WiCE(const WiCEOptions _engineOptions);
};

#endif // WiCE_Core_HPP