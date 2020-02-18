#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

/// @Author Michael Rainsford Ryan
/// @Date 30/12/2019

#include <SFML/Graphics.hpp>
#include <stack>
#include <array>
#include "Globals.h"
#include "Tile.h"

/// <summary>
/// @brief A static class used for generating a maze into a 2D array
/// </summary>
class MazeGenerator
{
public:
	static void generateMaze(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze);
};

#endif // !MAZE_GENERATOR_H