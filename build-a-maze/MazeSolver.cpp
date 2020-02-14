#include "MazeSolver.h"

/// @Author Michael Rainsford Ryan
/// @Date 28/09/2019

void MazeSolver::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_body);
}

void MazeSolver::setPos(int t_row, int t_col)
{
	m_pos.x = t_col; // Set the column
	m_pos.y = t_row; // Set the row

	m_previousPos = m_pos; // Set the previous position for animation
	m_body.setPosition(static_cast<sf::Vector2f>(m_pos * static_cast<int>(TILE_SIZE))); // Set the position to the current cell
}

void MazeSolver::move(TileType t_maze[][MAZE_COLS], sf::Vector2i t_newPosition)
{
	// Set the movement speed
	m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);

	// Replace the movement speed if moving through mud
	if (t_maze[m_pos.y][m_pos.x] == TileType::Mud
		|| t_maze[t_newPosition.y][t_newPosition.x] == TileType::Mud)
	{
		m_movementSpeed = static_cast<int>(SLOW_MOVE_SPEED * m_timeModifier);
	}

	m_pos = t_newPosition;

	handleTreadmills(t_maze);
}

void MazeSolver::findNewDirection(TileType t_maze[][MAZE_COLS])
{
	sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

	// If front blocked, always go left or right before going back
	// Both positive and negative (T junction)
	if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != TileType::Wall
		&& t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != TileType::Wall)
	{
		if (rand() % 2 == 0)
		{
			m_moveDir = Global::getDirection({ dir.y, dir.x });
		}
		else
		{
			m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
		}
	}

	// Positive
	else if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != TileType::Wall)
	{
		m_moveDir = Global::getDirection({ dir.y, dir.x });
	}

	// Negative
	else if (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != TileType::Wall)
	{
		m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
	}
	else {
		m_moveDir = static_cast<Direction>(rand() % 4 + 1); // Find a new direction
	}
}

void MazeSolver::reset(int t_moveDelay)
{
	setPos(1, 0);
	m_active = true;
	m_moveTimer = t_moveDelay;
	m_characterDirection = -100; // TEMP: Sloppy fix but works for now
	setTimeModifier(1);
}

void MazeSolver::checkForExit(TileType t_maze[][MAZE_COLS])
{
	// Check Vertically
	if (m_pos.y > MAZE_ROWS - 5 && m_pos.x == MAZE_COLS - 2)
	{
		bool goalBlocked = false;

		for (int i = m_pos.y; i < MAZE_ROWS - 1; i++)
		{
			if (static_cast<TileType>(t_maze[i][m_pos.x]) == TileType::Wall)
			{
				goalBlocked = true;
				break;
			}
		}
		if (goalBlocked == false)
		{
			m_moveDir = Direction::South;
		}
	}

	// Check horisontally
	if (m_pos.x > MAZE_COLS - 5 && m_pos.y == MAZE_ROWS - 2)
	{
		bool goalBlocked = false;

		for (int i = m_pos.x; i < MAZE_COLS; i++)
		{
			if (static_cast<TileType>(t_maze[m_pos.y][i]) == TileType::Wall)
			{
				goalBlocked = true;
				break;
			}
		}
		if (goalBlocked == false)
		{
			m_moveDir = Direction::East;
		}
	}
}

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

void MazeSolver::handleTreadmills(TileType t_maze[][MAZE_COLS])
{
	TileType tile{ TileType::None };

	// Current tile
	if (t_maze[m_previousPos.y][m_previousPos.x] == TileType::TreadmillWest
		|| t_maze[m_previousPos.y][m_previousPos.x] == TileType::TreadmillEast
		|| t_maze[m_previousPos.y][m_previousPos.x] == TileType::TreadmillNorth
		|| t_maze[m_previousPos.y][m_previousPos.x] == TileType::TreadmillSouth)
	{
		// Can be slowed and pushed aside
		tile = t_maze[m_previousPos.y][m_previousPos.x];

		// If the threadmill is going the same direction as the solver
		if (Global::getTreadmillDirection(tile) == m_moveDir)
		{
			m_movementSpeed = DEFAULT_MOVE_SPEED / 2;
		}
		// If the treadmill is going the opposite direction to the solver
		else if (Global::getDirectionVector(Global::getTreadmillDirection(tile)) == -Global::getDirectionVector(m_moveDir))
		{
			m_movementSpeed = SLOW_MOVE_SPEED;
		}
		else
		{
			sf::Vector2i directionVector = Global::getDirectionVector(Global::getTreadmillDirection(tile));

			if (t_maze[m_previousPos.y + directionVector.y][m_previousPos.x + directionVector.x] != TileType::Wall)
			{
				m_pos = m_previousPos + directionVector;
			}
		}
	}
	// Target tile
	else if (t_maze[m_pos.y][m_pos.x] == TileType::TreadmillWest
		|| t_maze[m_pos.y][m_pos.x] == TileType::TreadmillEast
		|| t_maze[m_pos.y][m_pos.x] == TileType::TreadmillNorth
		|| t_maze[m_pos.y][m_pos.x] == TileType::TreadmillSouth)
	{
		// Can be slowed and pushed aside
		tile = t_maze[m_pos.y][m_pos.x];

		// If the threadmill is going the same direction as the solver
		if (Global::getTreadmillDirection(tile) == m_moveDir)
		{
			m_movementSpeed = DEFAULT_MOVE_SPEED / 2;
		}
		// If the treadmill is going the opposite direction to the solver
		else if (Global::getDirectionVector(Global::getTreadmillDirection(tile)) == -Global::getDirectionVector(m_moveDir))
		{
			m_movementSpeed = SLOW_MOVE_SPEED;
		}
	}
}
