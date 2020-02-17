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
	Cartographer(std::array<std::array<TileType, MAZE_SIZE>, MAZE_SIZE> const& t_maze);
	void loadFiles();
	void update(); // Move the enemy if not blocked by an enemy or wall
	virtual void move(sf::Vector2i t_newPosition) override; // Move the solver to a new position
	virtual void findNewDirection() override; // Finds a new direction (direction solver is not facing). Always goes right or left before turning around
	virtual bool isBlocked(sf::Vector2i t_mazePos) override; // Check if a tile is a wall
	virtual void reset(int t_moveDelay) override;
	virtual void draw(sf::RenderWindow& t_window) const override;

protected:
	// Declare private data members
	static const int s_DEFAULT_MOVE_SPEED = 16;
	static const int s_SLOW_MOVE_SPEED = 32;
	std::stack<sf::Vector2i> m_previousTiles;
	bool m_deadEnds[MAZE_SIZE][MAZE_SIZE];
};

#endif // !CARTOGRAPHER_H