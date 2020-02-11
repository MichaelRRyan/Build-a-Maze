/// @Author Michael Rainsford Ryan
#include "Cartographer.h"

/// <summary>
/// <para>Enemy default constructer.</para>
/// <para>Load texture files, set the move direction, set the move timer,</para>
/// <para>set the sight range and following player bool</para>
/// </summary>
Cartographer::Cartographer()
{
	loadFiles();
	m_moveDir = static_cast<Direction>(rand() % 4 + 1);
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
	m_characterNumber = { 54, 64 * 19 };

	if (!m_spriteSheet.loadFromFile("ASSETS\\IMAGES\\characters.png"))
	{
		// Error loading image
	}
	m_body.setTexture(m_spriteSheet); // Set the character texture
	m_body.setTextureRect(sf::IntRect{ m_characterNumber.x * 2, m_characterNumber.y, 32, 64 }); // Set the character
	m_body.setOrigin(0.0f, static_cast<float>(32)); // Set the origin of the sprite to ignore the head part of the sprite
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
void Cartographer::update(TileType t_maze[][MAZE_COLS])
{
	if (m_pos.x == MAZE_COLS - 1 && m_pos.y == MAZE_ROWS - 2)
	{
		m_active = false;
	}

	if (m_moveTimer <= 0) // The enemy can only move once its timer reaches zero
	{
		// Check for new pathways on all sides
		sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

		// Positive
		if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != TileType::Wall
			&& !m_deadEnds[m_pos.y + dir.x][m_pos.x + dir.y]
			&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top()))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y, dir.x });
			}
		}

		// Negative
		if (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != TileType::Wall
			&& !m_deadEnds[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)]
			&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top()))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
			}
		}

		// Check for the exit nearby
		checkForExit(t_maze);

		m_previousPos = m_pos; // Set the previous position to the current one before moving

		for (int i = 0; i < 4; i++) // Loop until the enemy moves, finds a new direction or it tries four times (to stop infinite loops)
		{
			sf::Vector2i desiredPosition = m_pos + Global::getDirectionVector(m_moveDir); // Find the desired position from the current position and direction

			// Move if not blocked by a wall and not a dead end, else change direction
			if (t_maze[desiredPosition.y][desiredPosition.x] != TileType::Wall
				&& !m_deadEnds[desiredPosition.y][desiredPosition.x]
				&& (m_previousTiles.empty() || desiredPosition != m_previousTiles.top()))
			{
				move(t_maze, desiredPosition);
				break; // Break from the loop if the enemy can move
			}
			else
			{
				findNewDirection(t_maze);

				desiredPosition = m_pos + Global::getDirectionVector(m_moveDir);
				// Move if not blocked by a wall and not a dead end, else change direction
				if (t_maze[desiredPosition.y][desiredPosition.x] != TileType::Wall
					&& !m_deadEnds[desiredPosition.y][desiredPosition.x])
				{
					move(t_maze, desiredPosition);
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

void Cartographer::move(TileType t_maze[][MAZE_COLS], sf::Vector2i t_newPosition)
{
	if (static_cast<TileType>(t_maze[m_pos.y][m_pos.x]) == TileType::Mud
		|| static_cast<TileType>(t_maze[t_newPosition.y][t_newPosition.x]) == TileType::Mud)
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
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
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

void Cartographer::findNewDirection(TileType t_maze[][MAZE_COLS])
{
	sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

	// If front blocked, always go left or right before going back
	// Both positive and negative (T junction)
	if ((t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != TileType::Wall
		&& !m_deadEnds[m_pos.y + dir.x][m_pos.x + dir.y]
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top()))
		&& (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != TileType::Wall
		&& !m_deadEnds[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)]
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top())))
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
	else if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != TileType::Wall
		&& !m_deadEnds[m_pos.y + dir.x][m_pos.x + dir.y]
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + dir.y, m_pos.y + dir.x } != m_previousTiles.top()))
	{
		m_moveDir = Global::getDirection({ dir.y, dir.x });
	}
	// Negative
	else if (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != TileType::Wall
		&& !m_deadEnds[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)]
		&& (m_previousTiles.empty() || sf::Vector2i{ m_pos.x + (dir.y * -1), m_pos.y + (dir.x * -1) } != m_previousTiles.top()))
	{
		m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
	}
	// Pick a random direction
	else {
		m_moveDir = static_cast<Direction>(rand() % 4 + 1); // Find a new direction
	}
}

void Cartographer::reset(int t_moveDelay)
{
	setPos(1, 0);
	m_active = true;
	m_moveTimer = t_moveDelay;
	m_characterDirection = -100; // TEMP: Sloppy fix but works for now
	setTimeModifier(1);

	// Loop and clear the previous tiles stack
	while (!m_previousTiles.empty())
	{
		m_previousTiles.pop();
	}

	// Setup dead ends array
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			m_deadEnds[row][col] = false;
		}
	}
}
