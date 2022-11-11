#include "bitboard.hpp"

// ---------------------------------------------
// BitBoard
// ---------------------------------------------

const uint32_t BitBoard::BitTable[64] = {
	63, 30,  3, 32, 25, 41, 22, 33,
  	15, 50, 42, 13, 11, 53, 19, 34,
  	61, 29,  2, 51, 21, 43, 45, 10,
  	18, 47,  1, 54,  9, 57,  0, 35,
  	62, 31, 40,  4, 49,  5, 52, 26,
  	60,  6, 23, 44, 46, 27, 56, 16,
  	 7, 39, 48, 24, 59, 14, 12, 55,
  	38, 28, 58, 20, 37, 17, 36,  8
};

const uint64_t BitBoard::SetMask[64] = {
	0x0000000000000001ull, 0x0000000000000002ull, 
	0x0000000000000004ull, 0x0000000000000008ull, 
	0x0000000000000010ull, 0x0000000000000020ull, 
	0x0000000000000040ull, 0x0000000000000080ull, 
	0x0000000000000100ull, 0x0000000000000200ull, 
	0x0000000000000400ull, 0x0000000000000800ull, 
	0x0000000000001000ull, 0x0000000000002000ull, 
	0x0000000000004000ull, 0x0000000000008000ull, 
	0x0000000000010000ull, 0x0000000000020000ull, 
	0x0000000000040000ull, 0x0000000000080000ull, 
	0x0000000000100000ull, 0x0000000000200000ull, 
	0x0000000000400000ull, 0x0000000000800000ull, 
	0x0000000001000000ull, 0x0000000002000000ull, 
	0x0000000004000000ull, 0x0000000008000000ull, 
	0x0000000010000000ull, 0x0000000020000000ull, 
	0x0000000040000000ull, 0x0000000080000000ull, 
	0x0000000100000000ull, 0x0000000200000000ull, 
	0x0000000400000000ull, 0x0000000800000000ull, 
	0x0000001000000000ull, 0x0000002000000000ull, 
	0x0000004000000000ull, 0x0000008000000000ull, 
	0x0000010000000000ull, 0x0000020000000000ull, 
	0x0000040000000000ull, 0x0000080000000000ull, 
	0x0000100000000000ull, 0x0000200000000000ull, 
	0x0000400000000000ull, 0x0000800000000000ull, 
	0x0001000000000000ull, 0x0002000000000000ull, 
	0x0004000000000000ull, 0x0008000000000000ull, 
	0x0010000000000000ull, 0x0020000000000000ull, 
	0x0040000000000000ull, 0x0080000000000000ull, 
	0x0100000000000000ull, 0x0200000000000000ull, 
	0x0400000000000000ull, 0x0800000000000000ull, 
	0x1000000000000000ull, 0x2000000000000000ull, 
	0x4000000000000000ull, 0x8000000000000000ull
};

const uint64_t BitBoard::ClearMask[64] = {
	0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFDull, 
	0xFFFFFFFFFFFFFFFBull, 0xFFFFFFFFFFFFFFF7ull, 
	0xFFFFFFFFFFFFFFEFull, 0xFFFFFFFFFFFFFFDFull, 
	0xFFFFFFFFFFFFFFBFull, 0xFFFFFFFFFFFFFF7Full, 
	0xFFFFFFFFFFFFFEFFull, 0xFFFFFFFFFFFFFDFFull, 
	0xFFFFFFFFFFFFFBFFull, 0xFFFFFFFFFFFFF7FFull, 
	0xFFFFFFFFFFFFEFFFull, 0xFFFFFFFFFFFFDFFFull, 
	0xFFFFFFFFFFFFBFFFull, 0xFFFFFFFFFFFF7FFFull, 
	0xFFFFFFFFFFFEFFFFull, 0xFFFFFFFFFFFDFFFFull, 
	0xFFFFFFFFFFFBFFFFull, 0xFFFFFFFFFFF7FFFFull, 
	0xFFFFFFFFFFEFFFFFull, 0xFFFFFFFFFFDFFFFFull, 
	0xFFFFFFFFFFBFFFFFull, 0xFFFFFFFFFF7FFFFFull, 
	0xFFFFFFFFFEFFFFFFull, 0xFFFFFFFFFDFFFFFFull, 
	0xFFFFFFFFFBFFFFFFull, 0xFFFFFFFFF7FFFFFFull, 
	0xFFFFFFFFEFFFFFFFull, 0xFFFFFFFFDFFFFFFFull, 
	0xFFFFFFFFBFFFFFFFull, 0xFFFFFFFF7FFFFFFFull, 
	0xFFFFFFFEFFFFFFFFull, 0xFFFFFFFDFFFFFFFFull, 
	0xFFFFFFFBFFFFFFFFull, 0xFFFFFFF7FFFFFFFFull, 
	0xFFFFFFEFFFFFFFFFull, 0xFFFFFFDFFFFFFFFFull, 
	0xFFFFFFBFFFFFFFFFull, 0xFFFFFF7FFFFFFFFFull, 
	0xFFFFFEFFFFFFFFFFull, 0xFFFFFDFFFFFFFFFFull, 
	0xFFFFFBFFFFFFFFFFull, 0xFFFFF7FFFFFFFFFFull, 
	0xFFFFEFFFFFFFFFFFull, 0xFFFFDFFFFFFFFFFFull, 
	0xFFFFBFFFFFFFFFFFull, 0xFFFF7FFFFFFFFFFFull, 
	0xFFFEFFFFFFFFFFFFull, 0xFFFDFFFFFFFFFFFFull, 
	0xFFFBFFFFFFFFFFFFull, 0xFFF7FFFFFFFFFFFFull, 
	0xFFEFFFFFFFFFFFFFull, 0xFFDFFFFFFFFFFFFFull, 
	0xFFBFFFFFFFFFFFFFull, 0xFF7FFFFFFFFFFFFFull, 
	0xFEFFFFFFFFFFFFFFull, 0xFDFFFFFFFFFFFFFFull, 
	0xFBFFFFFFFFFFFFFFull, 0xF7FFFFFFFFFFFFFFull, 
	0xEFFFFFFFFFFFFFFFull, 0xDFFFFFFFFFFFFFFFull, 
	0xBFFFFFFFFFFFFFFFull, 0x7FFFFFFFFFFFFFFFull
};

BitBoard::BitBoard(
	const uint64_t _bb
) : bb(_bb) {}

BitBoard& BitBoard::operator=(
	const uint64_t _bb
) {
	bb = _bb;
	return *this;
}

BitBoard& BitBoard::operator|=(BitBoard& ob) {
	bb |= ob.bb;
	return *this;
}

BitBoard& BitBoard::operator|=(const uint64_t ob) {
	bb |= ob;
	return *this;
}

#ifdef DEBUG
#ifdef WiCE_Win
#pragma message("BitBoard::operator&() returns a uint64_t!")
#else
#warning BitBoard::operator&() returns a uint64_t!
#endif
#endif
const uint64_t BitBoard::operator&(BitBoard& ob) {
	return (bb & ob.bb);
}

BitBoard& BitBoard::clearBit(
	const int32_t sq
) {
	bb &= ClearMask[sq];
	return (*this);
}

BitBoard& BitBoard::setBit(
	const int32_t sq
) {
	bb |= SetMask[sq];
	return (*this);
}

const int32_t BitBoard::popBit() {
	uint64_t b = bb ^ (bb - 1);
	uint32_t fold =
		(uint32_t)((b & 0xffffffff) ^ (b >> 32));
	bb &= bb - 1;
	return BitTable[(fold * 0x783a9b23) >> 26];
}

const int32_t BitBoard::countBits() {
	uint64_t b = bb;
	int32_t i;
	for(i = 0; b; ++i, b &= b-1);
	return i;
}

const uint64_t BitBoard::get() {
	return bb;
}

const string BitBoard::toString() {
	string result = "";
	for(int32_t r = RANK_8; r >= RANK_1; --r) {
		for(int32_t f = FILE_A; f <= FILE_H; ++f) {
			int32_t sq = FR2SQ(f, r);
			int32_t sq64 = SQ64(sq);
			if((1ull << sq64) & bb)
				result.push_back('X');
			else
				result.push_back('-');
		}
		result.push_back('\n');
	}
	return result;
}

void BitBoard::print() {
	printf("\n%s", toString().c_str());
}