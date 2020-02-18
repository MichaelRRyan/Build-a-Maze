/// @Author Michael Rainsford Ryan
#ifndef BASIC_SOLVER_H
#define BASIC_SOLVER_H

#include <SFML\Graphics.hpp>
#include "Globals.h"
#include <iostream>
#include "MazeSolver.h"

class BasicSolver : public MazeSolver
{
public:
	BasicSolver(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze);
	void loadFiles();
	void update(); // Move the enemy if not blocked by an enemy or wall

protected:
	// Declare private data members
	const int DEFAULT_MOVE_SPEED = 16;
	const int SLOW_MOVE_SPEED = 32;
};

#endif // !BASIC_SOLVER_H