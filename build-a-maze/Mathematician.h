/// @Author Michael Rainsford Ryan
/// @Date 28/09/2019
#ifndef MATHEMATICIAN_H
#define MATHEMATICIAN_

#include <SFML\Graphics.hpp>
#include "Globals.h"
#include <iostream>
#include "MazeSolver.h"

class Mathematician : public MazeSolver
{
public:
	Mathematician();
	void loadFiles();
	void update(int t_maze[][MAZE_COLS]); // Move the enemy if not blocked by an enemy or wall

protected:
	// Declare private data members
	const int DEFAULT_MOVE_SPEED = 20;
	const int SLOW_MOVE_SPEED = 40;

	bool m_takeLefts; // True take all left turns, false take all right turns
};

#endif // !MATHEMATICIAN_H