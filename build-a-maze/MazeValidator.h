#ifndef MAZE_VALIDATOR_H
#define MAZE_VALIDATOR_H

//#define MAZE_VALIDATOR_DEBUG

#include <stack>
#include <array>
#include <iostream>
#include "Tile.h"

#ifdef MAZE_VALIDATOR_DEBUG
#include "Renderer.h"
#endif // MAZE_VALIDATOR_DEBUG

class MazeValidator
{
public:
	static bool isMazeSolvable(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze);
};

#endif // !MAZE_VALIDATOR_H