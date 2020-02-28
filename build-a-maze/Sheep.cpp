#include "Sheep.h"

///////////////////////////////////////////////////////////////////////////
Sheep::Sheep(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_maze) :
	m_mazeRef{ t_maze },
	m_followee{ nullptr }
{
	loadFiles();
	reset();

	setPos(1, 1);
	m_active = true;
}

///////////////////////////////////////////////////////////////////////////
void Sheep::update()
{
	// Decrement the move timer and animate
	if (m_moveTimer > 0)
	{
		m_moveTimer--;
		animate();
	}
	else
	{
		switch (m_state)
		{
		case Sheep::State::Idle:
			// Random 1 in 4 chance to switch to wander
			if (rand() % 4 == 0)
			{
				// Switch to wander and update wander to start
				m_state = State::Wander;
				updateWander();
			}
			else
			{
				m_movementSpeed = static_cast<int>(IDLE_SPEED * m_timeModifier);
			}

			break;
		case Sheep::State::Wander:
			// Random 1 in 10 chance of going back to idle
			if (rand() % 10 == 0)
			{
				m_state = State::Idle;
				m_movementSpeed = static_cast<int>(IDLE_SPEED * m_timeModifier);
			}
			else // Update wander if not going back to idle
			{
				updateWander();
			}

			break;
		case Sheep::State::Follow:

			if (m_followee->getActive()
				|| m_followee->getPos() == MAZE_EXIT)
			{
				m_previousPos = m_pos;
				m_pos = m_followee->getPreviousPos();
				m_movementSpeed = m_followee->getMovementSpeed();
				m_moveDir = Global::getDirection(m_pos - m_previousPos);

				// Check if the sheep made it to the end of the maze while following a solver
				if (m_pos.x == MAZE_SIZE - 2 && m_pos.y == MAZE_SIZE - 2)
				{
					m_active = false;
				}

			}
			else
			{
				m_followee = nullptr;
				m_state = State::Idle;
				m_movementSpeed = static_cast<int>(IDLE_SPEED * m_timeModifier);
			}

			break;
		}

		m_moveTimer = m_movementSpeed;
		setTextureDirection();
	}
}

///////////////////////////////////////////////////////////////////////////
void Sheep::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_sprite);
}

///////////////////////////////////////////////////////////////////////////
void Sheep::loadFiles()
{
	if (!m_spriteSheet.loadFromFile("ASSETS\\IMAGES\\character_sheet.png"))
	{
		throw("Error loading sheep image");
	}

	m_sprite.setTexture(m_spriteSheet); // Set the character texture
	m_sprite.setTextureRect({ 32, 640, 32, 64 }); // Set the character
	m_sprite.setOrigin(0.0f, 32.0f); // Set the origin of the sprite to ignore the head part of the sprite
}

///////////////////////////////////////////////////////////////////////////
void Sheep::reset()
{
	m_moveTimer = 0;
	m_characterDirection = 0;
	m_moveDir = Direction::South;
	m_state = State::Idle;
	m_movementSpeed = IDLE_SPEED;
	m_timeModifier = 1.0f;
	m_followee = nullptr;
}

///////////////////////////////////////////////////////////////////////////
void Sheep::setFollowing(MazeSolver* t_followee)
{
	// Set the followee and state to following if not already following a solver
	if (State::Follow != m_state)
	{
		m_followee = t_followee;
		m_state = State::Follow;
		m_moveTimer = 0;
	}
}

///////////////////////////////////////////////////////////////////////////
void Sheep::setPos(int t_row, int t_col)
{
	m_pos.x = t_col; // Set the column
	m_pos.y = t_row; // Set the row

	m_previousPos = m_pos; // Set the previous position for animation
	m_sprite.setPosition(static_cast<sf::Vector2f>(m_pos * static_cast<int>(TILE_SIZE))); // Set the position to the current cell
}

///////////////////////////////////////////////////////////////////////////
void Sheep::setTimeModifier(float t_mod)
{
	m_timeModifier = t_mod;

	switch (m_state)
	{
	case Sheep::State::Idle:
		m_movementSpeed = static_cast<int>(IDLE_SPEED * m_timeModifier);
		break;
	case Sheep::State::Wander:
		m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);
		break;
	}

	if (m_moveTimer > m_movementSpeed)
	{
		m_moveTimer = m_movementSpeed;
	}
}

///////////////////////////////////////////////////////////////////////////
const sf::Vector2i Sheep::getPos() const
{
	return m_pos;
}

///////////////////////////////////////////////////////////////////////////
const bool Sheep::getActive() const
{
	return m_active;
}

///////////////////////////////////////////////////////////////////////////
const sf::Sprite Sheep::getSprite() const
{
	return m_sprite;
}

///////////////////////////////////////////////////////////////////////////
const bool Sheep::isFollowing() const
{
	return State::Follow == m_state;
}

///////////////////////////////////////////////////////////////////////////
void Sheep::animate()
{
	// Grazing animation
	if (State::Idle == m_state)
	{
		int frameNum = static_cast<int>((1.0 * m_moveTimer / m_movementSpeed) * 4);
		sf::IntRect frame = { (32 * frameNum), 576, 32, 64 }; // Character height = 64, character width = 32

		m_sprite.setTextureRect(frame);
	}
	else // Movement animation
	{
		// Work out the new X and Y with Linear Interpolation
		float newX = (m_pos.x * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.x * 32) * (1.0f * m_moveTimer / m_movementSpeed);
		float newY = (m_pos.y * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.y * 32) * (1.0f * m_moveTimer / m_movementSpeed);

		// Set the position to the current cell
		m_sprite.setPosition(static_cast<float>(newX), static_cast<float>(newY));

		int frameNum = static_cast<int>((1.0 * m_moveTimer / m_movementSpeed) * 3);
		sf::IntRect frame = { (32 * frameNum), 640 + m_characterDirection * 64, 32, 64 }; // Character height = 64, character width = 32

		m_sprite.setTextureRect(frame);
	}
}

///////////////////////////////////////////////////////////////////////////
void Sheep::setTextureDirection()
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
		m_sprite.setScale(-1.0f, 1.0f);
		m_sprite.setOrigin(32.0f, m_sprite.getOrigin().y);
		break;
	case Direction::East: // Set the sprite to the look right texture
		m_characterDirection = 1;
		m_sprite.setScale(1.0f, 1.0f);
		m_sprite.setOrigin(0.0f, m_sprite.getOrigin().y);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
bool Sheep::isBlocked(sf::Vector2i t_mazePos)
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
void Sheep::updateWander()
{
	// Try 4 times to find a path to walk then give up
	bool directionFound = false;

	for (int i = 0; i < 4; i++)
	{
		// Pick a random direction
		Direction newDir = static_cast<Direction>(rand() % 4 + 1);
		sf::Vector2i newDirVec = Global::getDirectionVector(newDir);

		if (!isBlocked(m_pos + newDirVec))
		{
			directionFound = true;
			m_moveDir = newDir;

			// Assign the new and previous position variables
			m_previousPos = m_pos;
			m_pos += newDirVec;

			// Set the movement speed
			m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);

			// Replace the movement speed if moving through mud
			if (m_mazeRef[m_pos.y][m_pos.x] == TileType::Mud
				|| m_mazeRef[m_previousPos.y][m_previousPos.x] == TileType::Mud)
			{
				m_movementSpeed = static_cast<int>(SLOW_MOVE_SPEED * m_timeModifier);
			}

			handleTreadmills();

			break;
		}
	}

	if (!directionFound)
	{
		m_state = State::Idle;
		m_movementSpeed = static_cast<int>(IDLE_SPEED * m_timeModifier);
	}
}

///////////////////////////////////////////////////////////////////////////
void Sheep::handleTreadmills()
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