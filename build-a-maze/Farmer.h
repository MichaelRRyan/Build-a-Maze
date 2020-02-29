#ifndef FARMER_H
#define FARMER_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 29/02/2020
/// </summary>

#include <SFML\Graphics.hpp>
#include <array>
#include "Globals.h"
#include <iostream>
#include "MazeSolver.h"

class Farmer : public MazeSolver
{
public:
	Farmer(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_maze, std::vector<Sheep*>& t_sheepRef);
	void loadFiles();
	void update(); // Move the enemy if not blocked by an enemy or wall

	virtual void animate() override;

protected:

	// Declare private data members
	const int DEFAULT_MOVE_SPEED = 16;
	const int SLOW_MOVE_SPEED = 32;

	bool m_destroying;
};

#endif // !FARMER_H