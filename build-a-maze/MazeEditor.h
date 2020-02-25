#ifndef MAZE_EDITOR_H
#define MAZE_EDITOR_H

#include <array>
#include <vector>
#include "Globals.h"
#include "Tile.h"
#include "Cursor.h"

class MazeEditor
{
public:

	MazeEditor(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_mazeRef);

	void update(Cursor const & t_cursor, TileType t_selectedTileType, ConstructionMode t_constructionMode, int& t_currencyRef);

private:

	struct Action
	{
		TileType m_newTile;
		TileType m_prevTile;
		sf::Vector2i m_position; // Col, row format
		int m_moneySpent;
	};

	void addAction(TileType t_newTile, TileType t_prevTile, sf::Vector2i t_position, int t_moneySpent);

	// Maze array reference
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & m_mazeRef;

	// All actions performed this build mode
	// Used for undo/redo
	std::vector<Action> m_actions;

	// Current position in the action list
	// Anything after this index is removed when a new action is added
	int m_currentActionIndex;
};

#endif // !MAZE_EDITOR_H