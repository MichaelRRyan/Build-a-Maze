/// @Author Michael Rainsford Ryan
#include "Cartographer.h"

/// <summary>
/// <para>Enemy default constructer.</para>
/// <para>Load texture files, set the move direction, set the move timer,</para>
/// <para>set the sight range and following player bool</para>
/// </summary>
Cartographer::Cartographer(std::array<std::array<TileType, MAZE_SIZE>, MAZE_SIZE> const& t_maze) :
	MazeSolver{ t_maze }
{
	loadFiles();
	m_moveDir = Direction::East;
	m_moveTimer = 0;
	m_active = true;
	m_movementSpeed = DEFAULT_MOVE_SPEED;
	m_timeModifier = 1.0f;
}

/// <summary>
/// Load the texture file and apply it to the sprite.
/// </summary>
void Cartographer::loadFiles()
{
	m_characterNumber = { 0, 64 * 6 };

	if (!m_spriteSheet.loadFromFile("ASSETS\\IMAGES\\character_sheet.png"))
	{
		// Error loading image
	}
	m_body.setTexture(m_spriteSheet); // Set the character texture
	m_body.setTextureRect(sf::IntRect{ m_characterNumber.x * 2, m_characterNumber.y, 32, 64 }); // Set the character
	m_body.setOrigin(0.0f, 32.0f); // Set the origin of the sprite to ignore the head part of the sprite
}

/// <summary>
/// <para>Move the enemy in its current move direction.</para>
/// <para>Checks each frame for a player and switch direction if found</para>
/// <para>Enemy has a one in six chance to change direction randomly.</para>
/// <para>Checks that the path isn't blocked by a wall or an enemy.</para>
/// <para>Changes direction if blocked, move in direction if not.</para>
/// <para>Set the texture direction and move timer.</para>
/// </summary>
/// <param name="t_maze">maze array</param>
void Cartographer::update()
{
	if (m_pos.x == MAZE_SIZE - 1 && m_pos.y == MAZE_SIZE - 2)
	{
		m_active = false;
	}

	if (m_moveTimer <= 0) // The enemy can only move once its timer reaches zero
	{
		// Check for new pathways on all sides
		sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

		// Positive
		if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x })
			&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top()))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y, dir.x });
			}
		}

		// Negative
		if (!isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x })
			&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top()))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ -dir.y, -dir.x });
			}
		}

		// Check for the exit nearby
		checkForExit();

		m_previousPos = m_pos; // Set the previous position to the current one before moving

		for (int i = 0; i < 4; i++) // Loop until the enemy moves, finds a new direction or it tries four times (to stop infinite loops)
		{
			sf::Vector2i desiredPosition = m_pos + Global::getDirectionVector(m_moveDir); // Find the desired position from the current position and direction

			// Move if not blocked by a wall and not a dead end, else change direction
			if (!isBlocked(desiredPosition)
				&& (m_previousTiles.empty() || desiredPosition != m_previousTiles.top()))
			{
				move(desiredPosition);
				break; // Break from the loop if the enemy can move
			}
			else
			{
				findNewDirection();

				desiredPosition = m_pos + Global::getDirectionVector(m_moveDir);
				// Move if not blocked by a wall and not a dead end, else change direction
				if (!isBlocked(desiredPosition))
				{
					move(desiredPosition);
					break; // Break from the loop if the enemy can move
				}
			}
		}

		setTextureDirection(); // Set the texture to the direction
		m_moveTimer = m_movementSpeed; // Reset the move timer
	}
	else
	{
		m_moveTimer--;
		animate();
	}
}

void Cartographer::move(sf::Vector2i t_newPosition)
{
	if (static_cast<TileType>(m_mazeRef[m_pos.y][m_pos.x]) == TileType::Mud
		|| static_cast<TileType>(m_mazeRef[t_newPosition.y][t_newPosition.x]) == TileType::Mud)
	{
		m_movementSpeed = static_cast<int>(SLOW_MOVE_SPEED * m_timeModifier);
	}
	else
	{
		m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);
	}

	// If we're retracing steps
	if (!m_previousTiles.empty() && t_newPosition == m_previousTiles.top())
	{
		m_previousTiles.pop();
		m_deadEnds[m_pos.y][m_pos.x] = true;
	}
	else
	{
		m_previousTiles.push(m_pos);
	}

	m_pos = t_newPosition;

	handleTreadmills();
}

void Cartographer::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_body);

#ifdef CARTOGRAPHER_DEBUG
	// Create a copy of the stack
	std::stack<sf::Vector2i> prevTiles{ m_previousTiles };

	// Create and setup a shape to draw the debug info
	sf::RectangleShape prevTilesShape({ TILE_SIZE, TILE_SIZE });
	prevTilesShape.setFillColor(sf::Color{ 255,0,0,100 });

	// Loop through the stack and draw each item
	while (!prevTiles.empty())
	{
		prevTilesShape.setPosition(static_cast<sf::Vector2f>(prevTiles.top())* TILE_SIZE);
		t_window.draw(prevTilesShape);
		prevTiles.pop();
	}

	// Change the colour of the shape
	prevTilesShape.setFillColor(sf::Color{ 255,0,255,100 });

	// Loop through the dead ends array
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		for (int col = 0; col < MAZE_SIZE; col++)
		{
			if (m_deadEnds[row][col])
			{
				prevTilesShape.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				t_window.draw(prevTilesShape);
			}
		}
	}
#endif // CARTOGRAPHER_DEBUG
}

void Cartographer::findNewDirection()
{
	sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

	// If front blocked, always go left or right before going back
	// Both positive and negative (T junction)
	if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x })
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top())
		&& !isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x })
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top()))
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
	else if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x })
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top()))
	{
		m_moveDir = Global::getDirection({ dir.y, dir.x });
	}
	// Negative
	else if (!isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x })
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top()))
	{
		m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
	}
	// Pick a random direction
	else {
		m_moveDir = static_cast<Direction>(rand() % 4 + 1); // Find a new direction
	}
}

bool Cartographer::isBlocked(sf::Vector2i t_mazePos)
{
	// If outside the maze bounds
	if (t_mazePos.x < 0 || t_mazePos.x >= MAZE_SIZE
		|| t_mazePos.y < 0 || t_mazePos.y >= MAZE_SIZE)
	{
		return true;
	}

	// If blocked by a wall
	if (m_mazeRef[t_mazePos.y][t_mazePos.x] == TileType::Wall)
	{
		return true;
	}

	// If blocked by a dead end
	if (m_deadEnds[t_mazePos.y][t_mazePos.x])
	{
		return true;
	}

	return false;
}

void Cartographer::reset(int t_moveDelay)
{
	setPos(1, 0);
	m_active = true;
	m_moveTimer = t_moveDelay;
	m_characterDirection = -100; // TEMP: Sloppy fix but works for now
	m_moveDir = Direction::East;
	setTimeModifier(1);

	// Loop and clear the previous tiles stack
	while (!m_previousTiles.empty())
	{
		m_previousTiles.pop();
	}

	// Setup dead ends array
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		for (int col = 0; col < MAZE_SIZE; col++)
		{
			m_deadEnds[row][col] = false;
		}
	}
}