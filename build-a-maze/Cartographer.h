/// @Author Michael Rainsford Ryan
#ifndef CARTOGRAPHER_H
#define CARTOGRAPHER_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <stack>
#include "Globals.h"
#include "MazeSolver.h"


class Cartographer : public MazeSolver
{
public:
	Cartographer();
	void loadFiles();
	void move(int t_maze[][MAZE_COLS]); // Move the enemy if not blocked by an enemy or wall
	std::stack<sf::Vector2i> getPreviousTiles();

protected:
	// Declare private data members
	static const int s_DEFAULT_MOVE_SPEED = 16;
	static const int s_SLOW_MOVE_SPEED = 32;
	std::stack<sf::Vector2i> m_previousTiles;
};

#endif // !CARTOGRAPHER_H