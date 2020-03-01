#include "MazeEditor.h"

/////////////////////////////////////////////////////////////////
MazeEditor::MazeEditor(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_mazeRef, std::vector<Sheep*>& t_sheepRef, int& t_moneyRef) :
	m_mazeRef{ t_mazeRef },
	m_moneyRef{ t_moneyRef },
	m_sheepRef{ t_sheepRef },
	m_currentActionIndex{ 0 },
	m_constructionMode{ ConstructionMode::None },
	m_selectedTileType{ TileType::None }
{
}

/////////////////////////////////////////////////////////////////
void MazeEditor::update(Cursor const& t_cursor)
{
	if (t_cursor.m_clickDown)
	{
		// Make sure the player doesn't remove the outer boundary
		if (t_cursor.m_selectedTile.x > 0 && t_cursor.m_selectedTile.x < MAZE_SIZE - 1
			&& t_cursor.m_selectedTile.y > 0 && t_cursor.m_selectedTile.y < MAZE_SIZE - 1)
		{
			TileType selectedTile = m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].getType();

			// Check if a turret was clicked
			if ((selectedTile == TileType::TurretWest
				|| selectedTile == TileType::TurretEast)
				&& m_constructionMode == ConstructionMode::Destroying)
			{
				addAction(TileType::Wall, selectedTile, t_cursor.m_selectedTile, -Global::getTilePrice(selectedTile) / 2); // Note: Negative money spent

				m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(TileType::Wall);
				m_moneyRef += Global::getTilePrice(selectedTile) / 2;
			}
			// Check if the player clicked a tile
			else if (selectedTile != TileType::None
				&& m_constructionMode == ConstructionMode::Destroying)
			{
				addAction(TileType::None, selectedTile, t_cursor.m_selectedTile, -Global::getTilePrice(selectedTile) / 2); // Note: Negative money spent

				m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(TileType::None);
				m_moneyRef += Global::getTilePrice(selectedTile) / 2;
			}
			// Else the player tries to buy a tile
			else if (m_constructionMode == ConstructionMode::Placing)
			{
				// Check if the player has enough money for the selected item
				if (m_moneyRef >= Global::getTilePrice(m_selectedTileType))
				{
					if (selectedTile == TileType::None
						&& m_selectedTileType != TileType::TurretWest
						&& m_selectedTileType != TileType::TurretEast)
					{
						// Check if the tile is blocked by a sheep before placing
						bool blockedBySheep = false;

						for (Sheep* sheep : m_sheepRef)
						{
							if (sheep->getPos() == t_cursor.m_selectedTile)
							{
								blockedBySheep = true;
								break;
							}
						}

						if (!blockedBySheep)
						{
							addAction(m_selectedTileType, TileType::None, t_cursor.m_selectedTile, Global::getTilePrice(m_selectedTileType));

							m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(m_selectedTileType);
							m_moneyRef -= Global::getTilePrice(m_selectedTileType);
						}
					}
					else if (selectedTile == TileType::Wall
						&& (m_selectedTileType == TileType::TurretWest || m_selectedTileType == TileType::TurretEast))
					{
						addAction(m_selectedTileType, TileType::Wall, t_cursor.m_selectedTile, Global::getTilePrice(m_selectedTileType));

						m_mazeRef[t_cursor.m_selectedTile.y][t_cursor.m_selectedTile.x].setType(m_selectedTileType);
						m_moneyRef -= Global::getTilePrice(m_selectedTileType);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::handleKeyPresses(sf::Event t_event)
{
	// If Z key is pressed
	if (sf::Keyboard::Z == t_event.key.code)
	{
		// if the left control key is down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			// If the shift key is also down
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				redoAction();
			}
			else // If the shift key is not down
			{
				undoAction();
			}
		}
	}

	// If the R key is pressed
	if (sf::Keyboard::R == t_event.key.code)
	{
		rotateTile();
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::undoAction()
{
	// If there are actions to undo
	if (m_currentActionIndex > 0)
	{
		// Decrement first as the index starts from zero
		m_currentActionIndex--;

		// Check if the action was a sheep purchase
		if (m_actions.at(m_currentActionIndex).m_sheep)
		{
			// Save the sheep to a vector in case of redo
			m_deletedSheep.push_back(m_sheepRef.at(m_sheepRef.size() - 1));

			// Remove the sheep from the player's vector
			m_sheepRef.pop_back();

			// Add the player's money back
			m_moneyRef += SHEEP_PRICE;
		}
		else
		{
			// Replace the previous tile
			sf::Vector2i position = m_actions.at(m_currentActionIndex).m_position;
			m_mazeRef[position.y][position.x].setType(m_actions.at(m_currentActionIndex).m_prevTile);

			// Add/remove player's money back to before the transaction
			m_moneyRef += m_actions.at(m_currentActionIndex).m_moneySpent;
		}
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::redoAction()
{
	// If we are behind on actions that we can redo
	if (m_currentActionIndex < m_actions.size())
	{
		// Check if the action was a sheep purchase
		if (m_actions.at(m_currentActionIndex).m_sheep)
		{
			// Re-add the sheep from the top of the deleted sheep vector
			m_sheepRef.push_back(m_deletedSheep.at(m_deletedSheep.size() - 1));

			// Remove the sheep from the deleted sheep vector
			m_deletedSheep.pop_back();

			// remove the player's money for the sheep
			m_moneyRef -= SHEEP_PRICE;
		}
		else
		{
			sf::Vector2i position = m_actions.at(m_currentActionIndex).m_position;
			m_mazeRef[position.y][position.x].setType(m_actions.at(m_currentActionIndex).m_newTile);

			// Add/remove player's money back to after the transaction
			m_moneyRef -= m_actions.at(m_currentActionIndex).m_moneySpent;
		}

		// Increment by one action
		m_currentActionIndex++;
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::setSelectedTileType(TileType t_tileType)
{
	m_selectedTileType = t_tileType; // Set the selected tile type

	// If tile is null set the construction state to none
	if (TileType::None == t_tileType)
	{
		m_constructionMode = ConstructionMode::None;
	}
	else // If tile is not null, constuction state is placing
	{
		m_constructionMode = ConstructionMode::Placing;
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::enableDestroyMode()
{
	m_constructionMode = ConstructionMode::Destroying;
	m_selectedTileType = TileType::None;
}

/////////////////////////////////////////////////////////////////
void MazeEditor::unselectEditTool()
{
	m_constructionMode = ConstructionMode::None;
	m_selectedTileType = TileType::None;
}

/////////////////////////////////////////////////////////////////
const TileType MazeEditor::getSelectedTileType() const
{
	return m_selectedTileType;
}

/////////////////////////////////////////////////////////////////
const ConstructionMode MazeEditor::getConstructionMode() const
{
	return m_constructionMode;
}

/////////////////////////////////////////////////////////////////
void MazeEditor::reset()
{
	m_constructionMode = ConstructionMode::None;
	m_selectedTileType = TileType::None;
	m_actions.clear();
	m_currentActionIndex = 0;

	// Delete and remove all deleted sheep
	for (Sheep* sheep : m_deletedSheep)
	{
		delete sheep;
	}

	m_deletedSheep.clear();
}

/////////////////////////////////////////////////////////////////
void MazeEditor::purchaseSheep()
{
	// Remove all recent actions while the action count is above the current index (no longer able to redo)
	while (m_actions.size() > m_currentActionIndex)
	{
		// Check if the top action is a sheep action
		if (m_actions.at(m_actions.size() - 1).m_sheep)
		{
			// Delete and remove a sheep from deleted sheep vector if the action is a sheep action
			delete m_deletedSheep.at(m_deletedSheep.size() - 1);
			m_deletedSheep.pop_back();
		}

		m_actions.pop_back();
	}

	// Setup the new action
	Action action;

	action.m_sheep = true;

	// Push the new action onto the actions vector
	m_actions.push_back(action);

	// Keep the current action index up-to-date
	m_currentActionIndex = m_actions.size();
}

/////////////////////////////////////////////////////////////////
void MazeEditor::rotateTile()
{
	switch (m_selectedTileType)
	{
	case TileType::None: // Do nothing if no tile selected
		break;
	case TileType::TreadmillWest:
		m_selectedTileType = TileType::TreadmillNorth;
		break;
	case TileType::TreadmillEast:
		m_selectedTileType = TileType::TreadmillSouth;
		break;
	case TileType::TreadmillNorth:
		m_selectedTileType = TileType::TreadmillEast;
		break;
	case TileType::TreadmillSouth:
		m_selectedTileType = TileType::TreadmillWest;
		break;
	case TileType::TurretWest:
		m_selectedTileType = TileType::TurretEast;
		break;
	case TileType::TurretEast:
		m_selectedTileType = TileType::TurretWest;
		break;
	}
}

/////////////////////////////////////////////////////////////////
void MazeEditor::addAction(TileType t_newTile, TileType t_prevTile, sf::Vector2i t_position, int t_moneySpent)
{
	// Remove all recent actions while the action count is above the current index (no longer able to redo)
	while (m_actions.size() > m_currentActionIndex)
	{
		// Check if the top action is a sheep action
		if (m_actions.at(m_actions.size() - 1).m_sheep)
		{
			// Delete and remove a sheep from deleted sheep vector if the action is a sheep action
			delete m_deletedSheep.at(m_deletedSheep.size() - 1);
			m_deletedSheep.pop_back();
		}

		m_actions.pop_back();
	}

	// Setup the new action
	Action action;

	action.m_sheep = false;
	action.m_newTile = t_newTile;
	action.m_prevTile = t_prevTile;
	action.m_position = t_position;
	action.m_moneySpent = t_moneySpent;

	// Push the new action onto the actions vector
	m_actions.push_back(action);

	// Keep the current action index up-to-date
	m_currentActionIndex = m_actions.size();
}

/////////////////////////////////////////////////////////////////