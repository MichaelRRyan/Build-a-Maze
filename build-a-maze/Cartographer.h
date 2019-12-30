/// @Author Michael Rainsford Ryan
#ifndef CARTOGRAPHER_H
#define CARTOGRAPHER_H
//#define CARTOGRAPHER_DEBUG

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
	void update(TileType t_maze[][MAZE_COLS]); // Move the enemy if not blocked by an enemy or wall
	virtual void move(TileType t_maze[][MAZE_COLS], sf::Vector2i t_newPosition) override; // Move the solver to a new position
	virtual void findNewDirection(TileType t_maze[][MAZE_COLS]) override; // Finds a new direction (direction solver is not facing). Always goes right or left before turning around
	virtual void reset(int t_moveDelay) override;
	virtual void draw(sf::RenderWindow& t_window) const override;
	

protected:
	// Declare private data members
	static const int s_DEFAULT_MOVE_SPEED = 16;
	static const int s_SLOW_MOVE_SPEED = 32;
	std::stack<sf::Vector2i> m_previousTiles;
	bool m_deadEnds[MAZE_ROWS][MAZE_COLS];
};

#endif // !CARTOGRAPHER_H