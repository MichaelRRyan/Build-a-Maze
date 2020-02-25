#include "MazeEditor.h"

MazeEditor::MazeEditor(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_mazeRef) :
	m_mazeRef{ t_mazeRef },
	m_currentActionIndex{ 0 }
{
}

void MazeEditor::update(Cursor const& t_cursor, TileType t_selectedTileType, ConstructionMode t_constructionMode, int& t_currencyRef)
{
	if (t_cursor.m_clickDown)
	{
		// Make sure the player doesn't remove the outer boundary
		if (t_cursor.m_selectedTile.x > 0 && t_cursor.m_selectedTile.x < MAZE_SIZE - 1
			&& t_cursor.m_selectedTile.y > 0 && t_cursor.m_selectedTile.y < MAZE_SIZE - 1)
		{
			TileType selectedTile = m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].getType();

			// Check if a turret was clicked
			if (selectedTile == TileType::TurretWest
				|| selectedTile == TileType::TurretEast
				&& t_constructionMode == ConstructionMode::Destroying)
			{
				addAction(TileType::Wall, selectedTile, t_cursor.m_selectedTile, -Global::getTilePrice(selectedTile) / 2); // Note: Negative money spent

				m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(TileType::Wall);
				t_currencyRef += Global::getTilePrice(selectedTile) / 2;
			}
			// Check if the player clicked a tile
			else if (selectedTile != TileType::None
				&& t_constructionMode == ConstructionMode::Destroying)
			{
				addAction(TileType::None, selectedTile, t_cursor.m_selectedTile, -Global::getTilePrice(selectedTile) / 2); // Note: Negative money spent

				m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(TileType::None);
				t_currencyRef += Global::getTilePrice(selectedTile) / 2;
			}
			// Else the player tries to buy a tile
			else if (t_constructionMode == ConstructionMode::Placing)
			{
				// Check if the player has enough money for the selected item
				if (t_currencyRef >= Global::getTilePrice(t_selectedTileType))
				{
					if (selectedTile == TileType::None
						&& t_selectedTileType != TileType::TurretWest
						&& t_selectedTileType != TileType::TurretEast)
					{
						addAction(t_selectedTileType, TileType::None, t_cursor.m_selectedTile, Global::getTilePrice(t_selectedTileType));

						m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(t_selectedTileType);
						t_currencyRef -= Global::getTilePrice(t_selectedTileType);
					}
					else if (selectedTile == TileType::Wall
						&& (t_selectedTileType == TileType::TurretWest || t_selectedTileType == TileType::TurretEast))
					{
						addAction(t_selectedTileType, TileType::Wall, t_cursor.m_selectedTile, Global::getTilePrice(t_selectedTileType));

						m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(t_selectedTileType);
						t_currencyRef -= Global::getTilePrice(t_selectedTileType);
					}
				}
			}
		}
	}
}

void MazeEditor::addAction(TileType t_newTile, TileType t_prevTile, sf::Vector2i t_position, int t_moneySpent)
{
	// Remove all recent actions while the action count is above the current index (no longer able to redo)
	while (m_actions.size() > m_currentActionIndex)
	{
		m_actions.pop_back();
	}

	// Setup the new action
	Action action;

	action.m_newTile = t_newTile;
	action.m_prevTile = t_prevTile;
	action.m_position = t_position;
	action.m_moneySpent = t_moneySpent;

	// Push the new action onto the actions vector
	m_actions.push_back(action);

	// Keep the current action index up-to-date
	m_currentActionIndex = m_actions.size();
}
