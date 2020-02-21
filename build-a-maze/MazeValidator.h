#ifndef MAZE_VALIDATOR
#define MAZE_VALIDATOR

#include <stack>
#include <array>
#include <iostream>
#include "Tile.h"
#include "Renderer.h"

class MazeValidator
{
public:
	static bool isMazeSolvable(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze);
};

#endif // !MAZE_VALIDATOR