/// @Author Michael Rainsford Ryan

#ifndef GLOBALS_H
#define GLOBALS_H

const int MAZE_ROWS{ 15 };
const int MAZE_COLS{ 15 };
const float TILE_SIZE{ 32.0f };

const int BASIC_SOLVERS_MAX{ 6 };

const unsigned WINDOW_WIDTH{ 1280u };
const unsigned WINDOW_HEIGHT{ 720u };

enum class ConstructionMode
{
	None,
	Placing,
	Destroying
};


#endif // !GLOBALS_H

