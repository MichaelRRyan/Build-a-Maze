#ifndef MAZE_EDITOR_H
#define MAZE_EDITOR_H

#include <array>
#include <vector>
#include "Globals.h"
#include "Tile.h"
#include "Cursor.h"
#include "Sheep.h"

class MazeEditor
{
public:

	MazeEditor(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_mazeRef, std::vector<Sheep *> & t_sheepRef, int & t_moneyRef);

	void update(Cursor const & t_cursor);

	void handleKeyPresses(sf::Event t_event);

	void undoAction();

	void redoAction();

	void setSelectedTileType(TileType t_tileType);

	void enableDestroyMode();

	/// <summary>
	/// @Brief Sets the current constuction mode and selected tile type to 'none'
	/// </summary>
	void unselectEditTool();

	const TileType getSelectedTileType() const;

	const ConstructionMode getConstructionMode() const;

	/// <summary>
	/// @Brief Sets all values to default
	/// </summary>
	void reset();

	void purchaseSheep();

private:

	struct Action
	{
		bool m_sheep; // Whether the action is a sheep purchase or not
		TileType m_newTile;
		TileType m_prevTile;
		sf::Vector2i m_position; // Col, row format
		int m_moneySpent;
	};

	void addAction(TileType t_newTile, TileType t_prevTile, sf::Vector2i t_position, int t_moneySpent);

	// Maze array reference
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & m_mazeRef;

	// Player's money reference
	int & m_moneyRef;

	// Reference to the player's sheep
	std::vector<Sheep*> & m_sheepRef;

	// A vector to hold deleted sheep
	std::vector<Sheep*> m_deletedSheep;

	// All actions performed this build mode
	// Used for undo/redo
	std::vector<Action> m_actions;

	// Current position in the action list
	// Anything after this index is removed when a new action is added
	int m_currentActionIndex;

	// The player's currently selected tile type for building
	TileType m_selectedTileType;

	// The constuction mode the player is in (None, placing, destroying)
	ConstructionMode m_constructionMode;
};

#endif // !MAZE_EDITOR_H