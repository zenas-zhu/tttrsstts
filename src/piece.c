#include "piece.h"

const int PIECES_SIZE = 7;

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
