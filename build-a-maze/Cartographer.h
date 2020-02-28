/// @Author Michael Rainsford Ryan
#ifndef CARTOGRAPHER_H
#define CARTOGRAPHER_H
//#define CARTOGRAPHER_DEBUG

#include <SFML\Graphics.hpp>
#include <iostream>
#include <stack>
#include <array>
#include "Globals.h"
#include "MazeSolver.h"


class Cartographer : public MazeSolver
{
public:
	Cartographer(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze, std::vector<Sheep*>& t_sheepRef);
	void loadFiles();
	void update(); // Move the enemy if not blocked by an enemy or wall
	virtual void move(sf::Vector2i t_newPosition) override; // Move the solver to a new position
	virtual void findNewDirection() override; // Finds a new direction (direction solver is not facing). Always goes right or left before turning around
	virtual bool isBlocked(sf::Vector2i t_mazePos) const override; // Check if a tile is a wall
	virtual void reset(int t_moveDelay) override;
	virtual void draw(sf::RenderWindow& t_window) const override;
	virtual void hasFollower(bool t_hasFollower) override;

protected:

	bool isCornered() const; // Return true if no possible movements
	void clearMap(); // Clear the 'map'

	// Declare private data members
	static const int s_DEFAULT_MOVE_SPEED = 16;
	static const int s_SLOW_MOVE_SPEED = 32;
	std::stack<sf::Vector2i> m_previousTiles;
	std::array<std::array<bool, MAZE_SIZE>, MAZE_SIZE> m_deadEnds;
};

#endif // !CARTOGRAPHER_H