#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

/// @Author Michael Rainsford Ryan
/// @Date 30/12/2019

#include <SFML/Graphics.hpp>
#include <stack>
#include "Globals.h"

/// <summary>
/// @brief A static class used for generating a maze into a 2D array
/// </summary>
class MazeGenerator
{
public:
	static void generateMaze(TileType t_maze[][MAZE_COLS]);
};

#endif // !MAZE_GENERATOR_H