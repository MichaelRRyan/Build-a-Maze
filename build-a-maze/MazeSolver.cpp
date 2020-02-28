#include "MazeSolver.h"

/// @Author Michael Rainsford Ryan
/// @Date 28/09/2019

///////////////////////////////////////////////////////////////////////////
MazeSolver::MazeSolver(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze, std::vector<Sheep*>& t_sheepRef) :
	m_mazeRef{ t_maze },
	m_sheepRef{ t_sheepRef }
{
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_body);
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::setPos(int t_row, int t_col)
{
	m_pos.x = t_col; // Set the column
	m_pos.y = t_row; // Set the row

	m_previousPos = m_pos; // Set the previous position for animation
	m_body.setPosition(static_cast<sf::Vector2f>(m_pos * static_cast<int>(TILE_SIZE))); // Set the position to the current cell
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::move(sf::Vector2i t_newPosition)
{
	// Set the movement speed
	m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);

	// Replace the movement speed if moving through mud
	if (m_mazeRef[m_pos.y][m_pos.x] == TileType::Mud
		|| m_mazeRef[t_newPosition.y][t_newPosition.x] == TileType::Mud)
	{
		m_movementSpeed = static_cast<int>(SLOW_MOVE_SPEED * m_timeModifier);
	}

	m_pos = t_newPosition;

	handleTreadmills();
	handleSteppingStones();
	handleTrapdoors();
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::findNewDirection()
{
	sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

	// If front blocked, always go left or right before going back
	// Both positive and negative (T junction)
	if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x})
		&& !isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x }))
	{
		if (rand() % 2 == 0)
		{
			m_moveDir = Global::getDirection({ dir.y, dir.x });
		}
		else
		{
			m_moveDir = Global::getDirection({ -dir.y, -dir.x });
		}
	}

	// Positive
	else if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x }))
	{
		m_moveDir = Global::getDirection({ dir.y, dir.x });
	}

	// Negative
	else if (!isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x }))
	{
		m_moveDir = Global::getDirection({ -dir.y, -dir.x });
	}
	else {
		m_moveDir = static_cast<Direction>(rand() % 4 + 1); // Find a new direction
	}
}

///////////////////////////////////////////////////////////////////////////
bool MazeSolver::isBlocked(sf::Vector2i t_mazePos) const
{
	// If outside the maze bounds
	if (t_mazePos.x < 0 || t_mazePos.x >= MAZE_SIZE
		|| t_mazePos.y < 0 || t_mazePos.y >= MAZE_SIZE)
	{
		return true;
	}

	// If blocked by a wall
	if (m_mazeRef[t_mazePos.y][t_mazePos.x] == TileType::Wall
		|| m_mazeRef[t_mazePos.y][t_mazePos.x] == TileType::TurretWest
		|| m_mazeRef[t_mazePos.y][t_mazePos.x] == TileType::TurretEast)
	{
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::reset(int t_moveDelay)
{
	setPos(1, 0);
	m_active = true;
	m_moveTimer = t_moveDelay;
	m_characterDirection = -100; // TEMP: Sloppy fix but works for now
	m_moveDir = Direction::East;
	setTimeModifier(1);
	hasFollower(false);
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::checkForExit()
{
	// Only go towards exit if we have a sheep
	if (m_hasFollower)
	{
		// Check Vertically
		if (m_pos.y < 4 && m_pos.x == 1)
		{
			bool goalBlocked = false;

			for (int i = m_pos.y; i > 0; i--)
			{
				if (isBlocked({ m_pos.x, i }))
				{
					goalBlocked = true;
					break;
				}
			}
			if (goalBlocked == false)
			{
				m_moveDir = Direction::North;
			}
		}

		// Check horisontally
		if (m_pos.x < 4 && m_pos.y == 1)
		{
			bool goalBlocked = false;

			for (int i = m_pos.x; i > 0; i--)
			{
				if (isBlocked({ i, m_pos.y }))
				{
					goalBlocked = true;
					break;
				}
			}
			if (goalBlocked == false)
			{
				m_moveDir = Direction::West;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::animate()
{
	// Work out the new X and Y with Linear Interpolation
	float newX = (m_pos.x * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.x * 32) * (1.0f * m_moveTimer / m_movementSpeed);
	float newY = (m_pos.y * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.y * 32) * (1.0f * m_moveTimer / m_movementSpeed);

	// Set the position to the current cell
	m_body.setPosition(static_cast<float>(newX), static_cast<float>(newY));

	int frameNum = static_cast<int>((1.0 * m_moveTimer / m_movementSpeed) * 3);
	sf::IntRect frame = sf::IntRect{ m_characterNumber.x + (32 * frameNum), m_characterNumber.y + m_characterDirection * 64, 32, 64 }; // Character height = 64, character width = 32

	m_body.setTextureRect(frame);
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::setTextureDirection()
{
	switch (m_moveDir)
	{
	case Direction::North: // Set the sprite to the look up texture
		m_characterDirection = 2;
		break;
	case Direction::South: // Set the sprite to the look down texture
		m_characterDirection = 0;
		break;
	case Direction::West: // Set the sprite to the look left texture
		m_characterDirection = 1;
		m_body.setScale(-1.0f, 1.0f);
		m_body.setOrigin(static_cast<float>(32), m_body.getOrigin().y);
		break;
	case Direction::East: // Set the sprite to the look right texture
		m_characterDirection = 1;
		m_body.setScale(1.0f, 1.0f);
		m_body.setOrigin(0.0f, m_body.getOrigin().y);
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
const int MazeSolver::getMovementSpeed() const
{
	return m_movementSpeed;
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::hasFollower(bool t_hasFollower)
{
	m_hasFollower = t_hasFollower;
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::handleTreadmills()
{
	// Only check for treadmill if the tile is animating
	if (m_mazeRef[m_previousPos.y][m_previousPos.x].getAnimating())
	{
		// Current tile
		if (m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::TreadmillWest
			|| m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::TreadmillEast
			|| m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::TreadmillNorth
			|| m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::TreadmillSouth)
		{
			// Can be slowed and pushed aside
			TileType tile = m_mazeRef[m_previousPos.y][m_previousPos.x].getType();

			// If the threadmill is going the same direction as the solver
			if (Global::getDirection(tile) == m_moveDir)
			{
				m_movementSpeed = DEFAULT_MOVE_SPEED / 2;
			}
			// If the treadmill is going the opposite direction to the solver
			else if (Global::getDirectionVector(Global::getDirection(tile)) == -Global::getDirectionVector(m_moveDir))
			{
				m_movementSpeed = SLOW_MOVE_SPEED;
			}
			else
			{
				sf::Vector2i directionVector = Global::getDirectionVector(Global::getDirection(tile));

				if (m_mazeRef[m_previousPos.y + directionVector.y][m_previousPos.x + directionVector.x] != TileType::Wall)
				{
					m_pos = m_previousPos + directionVector;
				}
			}
		}
		// Target tile
		else if (m_mazeRef[m_pos.y][m_pos.x] == TileType::TreadmillWest
			|| m_mazeRef[m_pos.y][m_pos.x] == TileType::TreadmillEast
			|| m_mazeRef[m_pos.y][m_pos.x] == TileType::TreadmillNorth
			|| m_mazeRef[m_pos.y][m_pos.x] == TileType::TreadmillSouth)
		{
			// Can be slowed and pushed aside
			TileType tile = m_mazeRef[m_pos.y][m_pos.x].getType();

			// If the threadmill is going the same direction as the solver
			if (Global::getDirection(tile) == m_moveDir)
			{
				m_movementSpeed = DEFAULT_MOVE_SPEED / 2;
			}
			// If the treadmill is going the opposite direction to the solver
			else if (Global::getDirectionVector(Global::getDirection(tile)) == -Global::getDirectionVector(m_moveDir))
			{
				m_movementSpeed = SLOW_MOVE_SPEED;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::handleSteppingStones()
{
	if (m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::SteppingStones)
	{
		m_mazeRef[m_previousPos.y][m_previousPos.x].setAnimating(true);

		if (rand() % 5 == 0)
		{
			m_active = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::handleTrapdoors()
{
	if (m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::Trapdoor)
	{
		if (m_mazeRef[m_previousPos.y][m_previousPos.x].getAnimating())
		{
			m_active = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::checkForSheep()
{
	// Only check for sheep if we don't have a sheep and there are sheep available
	if (!m_hasFollower && sheepAvailable())
	{
		int sightRange = 5;
		int closestSheep = sightRange + 1;

		// Loop for the 4 directions (enum starts at index 1)
		for (int i = 1; i < 5; i++)
		{
			Direction direction = static_cast<Direction>(i);
			sf::Vector2i dirVec = Global::getDirectionVector(direction);

			// Loop for 5 tiles in each direction
			for (int j = 1; j <= sightRange; j++)
			{
				// Get the position from the solver position plus the direction times the current distance
				sf::Vector2i position = m_pos + (dirVec * j);

				if (isBlocked(position))
				{
					// Vision blocked, break
					break;
				}
				else
				{
					// Loop all sheep
					for (Sheep* sheep : m_sheepRef)
					{
						// Check that the sheep is active and isn't already following another solver
						if (sheep->getActive() && !sheep->isFollowing())
						{
							// Check if the sheep position matches the current tile
							if (sheep->getPos() == position)
							{
								// Check if this is the closest sheep
								if (j < closestSheep)
								{
									m_moveDir = direction; // Move towards sheep
									closestSheep = j; // Overwrite the closest sheep variable to find the closest sheep
								}
								break; // Found goal, break
							}
						}
					}
				}
			} // End inner for

			// If the closest sheep is 1 tile away, break, as that is as close as we can get
			if (1 == closestSheep)
			{
				break;
			}
		} // End outer for
	}
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::checkSheepCollisions()
{
	// Only check for sheep if the solver doesn't have a follower
	if (!m_hasFollower)
	{
		// Loop all sheep
		for (Sheep* sheep : m_sheepRef)
		{
			if (sheep->getActive() && !sheep->isFollowing())
			{
				// Check sprite collisions with the sheep
				if (m_pos == sheep->getPos())
				{
					sheep->setFollowing(this);
					hasFollower(true);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
bool MazeSolver::sheepAvailable()
{
	// Check if there are any available sheep (active sheep not following anyone)
	bool sheepAvailable{ false };
	for (Sheep* sheep : m_sheepRef)
	{
		if (sheep->getActive() && !sheep->isFollowing())
		{
			sheepAvailable = true;
			break;
		}
	}

	return sheepAvailable;
}

///////////////////////////////////////////////////////////////////////////
void MazeSolver::checkIfOutOfMaze()
{
	// Check if at the maze exit
	if (m_pos == MAZE_EXIT)
	{
		// Check if we have a follower or none are available
		if (m_hasFollower || !sheepAvailable())
		{
 			m_active = false; // No longer active, made it to the end of the game
		}
	}
}

///////////////////////////////////////////////////////////////////////////