/// @Author Michael Rainsford Ryan
#include "BasicSolver.h"


/// <summary>
/// <para>Enemy default constructer.</para>
/// <para>Load texture files, set the move direction, set the move timer,</para>
/// <para>set the sight range and following player bool</para>
/// </summary>
BasicSolver::BasicSolver()
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
void BasicSolver::loadFiles()
{
	m_characterNumber = { 54, 64 * 3 };

	if (!m_spriteSheet.loadFromFile("ASSETS\\IMAGES\\characters.png"))
	{
		// Error loading image
	}
	m_body.setTexture(m_spriteSheet); // Set the character texture
	m_body.setTextureRect(sf::IntRect{ m_characterNumber.x * 2,m_characterNumber.y,32,64 }); // Set the character
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
/// <param name="t_maze"></param>
/// <param name="t_ghosts"></param>
void BasicSolver::move(int t_maze[][MAZE_COLS])
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
		if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != 10)
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y, dir.x });
			}
		}

		// Negative
		if (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != 10)
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
			}
		}

		// Check for the exit nearby
		// Check Vertically
		if (m_pos.y > MAZE_ROWS - 5 && m_pos.x == MAZE_COLS - 2)
		{
			bool goalBlocked = false;

			for (int i = m_pos.y; i < MAZE_ROWS - 1; i++)
			{
				if (t_maze[i][m_pos.x] == TileType::Wall)
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
				if (t_maze[m_pos.y][i] == TileType::Wall)
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


		m_previousPos = m_pos; // Set the previous position to the current one before moving

		for (int i = 0; i < 4; i++) // Loop until the enemy moves, finds a new direction or it tries four times (to stop infinite loops)
		{
			sf::Vector2i desiredPosition = m_pos + Global::getDirectionVector(m_moveDir); // Find the desired position from the current position and direction
			bool blocked = false; // True if the desired position holds another enemy or wall

			if (t_maze[desiredPosition.y][desiredPosition.x] == 10
				|| (desiredPosition.y < 0 || desiredPosition.y > MAZE_ROWS || desiredPosition.x < 0 || desiredPosition.x > MAZE_COLS)) // Check if there's a rock blocking movement or trying to leave the maze
			{
				blocked = true; // Movement is blocked
			}

			// Move if not blocked, else change direction
			if (!blocked)
			{
				if (t_maze[m_pos.y][m_pos.x] == TileType::Slow
					|| t_maze[desiredPosition.y][desiredPosition.x] == TileType::Slow)
				{
					m_movementSpeed = static_cast<int>(SLOW_MOVE_SPEED * m_timeModifier);
				}
				else
				{
					m_movementSpeed = static_cast<int>(DEFAULT_MOVE_SPEED * m_timeModifier);
				}

				m_pos = desiredPosition;
				break; // Break from the loop if the enemy can move
			}
			else
			{
				// If front blocked, always go left or right before going back
				// Both positive and negative (T junction)
				if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != 10
					&& t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != 10)
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
				else if (t_maze[m_pos.y + dir.x][m_pos.x + dir.y] != 10)
				{
					m_moveDir = Global::getDirection({ dir.y, dir.x });
				}
				// Negative
				else if (t_maze[m_pos.y + (dir.x * -1)][m_pos.x + (dir.y * -1)] != 10) 
				{
					m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
				}
				else {
					m_moveDir = static_cast<Direction>(rand() % 4 + 1); // Find a new direction
				}
			}

		}

		setTextureDirection(); // Set the texture to the direction
		m_moveTimer = m_movementSpeed; // Reset the move timer
	}
	else
	{
		m_moveTimer--;
		// Work out the new X and Y with Linear Interpolation
		float newX = (m_pos.x * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.x * 32) * (1.0f * m_moveTimer / m_movementSpeed);
		float newY = (m_pos.y * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.y * 32) * (1.0f * m_moveTimer / m_movementSpeed);
		m_body.setPosition(static_cast<float>(newX), static_cast<float>(newY)); // Set the position to the current cell
		int frameNum = static_cast<int>((1.0 * m_moveTimer / m_movementSpeed) * 3);
		sf::IntRect frame = sf::IntRect{ m_characterNumber.x + (m_characterNumber.x * frameNum), m_characterNumber.y + m_characterDirection * 64, 32, 64 }; // Character height = 64, character width = 32
		m_body.setTextureRect(frame);
	}

}