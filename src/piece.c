#include "piece.h"

#define _ false,
#define O true,
static const bool PIECE_I[] = {
	_ _ _ _
	O O O O
	_ _ _ _
	_ _ _ _
};
static const bool PIECE_J[] = {
	O _ _
	O O O
	_ _ _
};
static const bool PIECE_L[] = {
	_ _ O
	O O O
	_ _ _
};
static const bool PIECE_O[] = {
	O O
	O O
};
static const bool PIECE_S[] = {
	_ O O
	O O _
	_ _ _
};
static const bool PIECE_T[] = {
	_ O _
	O O O
	_ _ _
};
static const bool PIECE_Z[] = {
	O O _
	_ O O
	_ _ _
};
#undef _
#undef O

const bool *PIECES[] = {
	PIECE_I,
	PIECE_J,
	PIECE_L,
	PIECE_O,
	PIECE_S,
	PIECE_T,
	PIECE_Z,
};

const int PIECE_SIZES[] = {
	4, 3, 3, 2, 3, 3, 3,
};

const int PIECE_COLORS[] = {
	44, 62, 172, 185, 40, 170, 167,
};

const int PIECE_COLORS_RGB[] = {
	0x0F9BD7, 0x2141C6, 0xE35B02, 0xE39F02, 0x59B101, 0xAF298A, 0xD70F37
};

const int KICKS_IDS[] = {
	1, 0, 0, 2, 0, 0, 0,
};

const int KICKS_SIZES[] = {
	4, 4, 0,
};

// all in format (x, y), i was too lazy to fix these for now
static const int KICKS_2X3[] = {
	// CW
	-1, 0, -1, +1, 0, -2, -1, -2,
	+1, 0, +1, -1, 0, +2, +1, +2,
	+1, 0, +1, +1, 0, -2, +1, -2,
	-1, 0, -1, -1, 0, +2, -1, +2,
	// 180 (some of tetrio's 180 kicks)
	0, +1, +1, +1, -1, +1,    0, -999,
	+1, 0, +1, +2, +1, +1,    0, -999,
	0, -1, -1, -1, +1, -1,    0, -999,
	-1, 0, -1, +2, -1, +1,    0, -999,
	// CCW
	+1, 0, +1, +1, 0, -2, +1, -2,
	+1, 0, +1, -1, 0, +2, +1, +2,
	-1, 0, -1, +1, 0, -2, -1, -2,
	-1, 0, -1, -1, 0, +2, -1, +2,
};

static const int KICKS_I[] = {
	// CW (stole from arika)
	-2, 0, +1, 0, +1, +2, -2, -1,
	-1, 0, +2, 0, -1, +2, +2, -1,
	+2, 0, -1, 0, +2, +1, -1, -1,
	-2, 0, +1, 0, -2, +1, +1, -2,
	// 180 (??)
	0, +1,    0, -999, 0, -999, 0, -999,
	+1, 0,    0, -999, 0, -999, 0, -999,
	0, -1,    0, -999, 0, -999, 0, -999,
	-1, 0,    0, -999, 0, -999, 0, -999,
	// CCW (stole from arika)
	+2, 0, -1, 0, -1, +2, +2, -1,
	+2, 0, -1, 0, +2, +1, -1, -2,
	-2, 0, +1, 0, -2, +1, +1, -1,
	+1, 0, -2, 0, +1, +2, -2, -1,
};

const int *KICKS[] = {
	KICKS_2X3,
	KICKS_I,
	(const int *)0, // no O kicks
};

const int T_ID = 5;
