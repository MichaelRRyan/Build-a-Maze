#ifndef MAZE_VALIDATOR_H
#define MAZE_VALIDATOR_H

//#define MAZE_VALIDATOR_DEBUG

#include <stack>
#include <array>
#include <vector>
#include <iostream>
#include "Tile.h"

#ifdef MAZE_VALIDATOR_DEBUG
#include "Renderer.h"
#endif // MAZE_VALIDATOR_DEBUG

class MazeValidator
{
public:
	bool isMazeSolvable(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze);

	std::vector<sf::Vector2i> const & getAccessibleTiles() const;

private:

	std::vector<sf::Vector2i> m_accessibleTiles;
};

#endif // !MAZE_VALIDATOR_H