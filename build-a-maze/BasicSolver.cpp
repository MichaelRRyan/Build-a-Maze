/// @Author Michael Rainsford Ryan
#include "BasicSolver.h"


/// <summary>
/// <para>Enemy default constructer.</para>
/// <para>Load texture files, set the move direction, set the move timer,</para>
/// <para>set the sight range and following player bool</para>
/// </summary>
BasicSolver::BasicSolver(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze) :
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
void BasicSolver::loadFiles()
{
	m_characterNumber = { 0, 0 };

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
/// <param name="t_maze"></param>
/// <param name="t_ghosts"></param>
void BasicSolver::update()
{
	if (m_pos.x == MAZE_SIZE - 1 && m_pos.y == MAZE_SIZE - 2)
	{
		m_active = false;
		return;
	}

	if (m_moveTimer <= 0) // The enemy can only move once its timer reaches zero
	{
		// Check for new pathways on all sides
		sf::Vector2i dir = Global::getDirectionVector(m_moveDir);

		// Positive
		if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x }))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y, dir.x });
			}
		}

		// Negative
		if (!isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x }))
		{
			if (rand() % 2 == 0) {
				m_moveDir = Global::getDirection({ dir.y * -1, dir.x * -1 });
			}
		}

		// Check for the exit nearby
		checkForExit();

		m_previousPos = m_pos; // Set the previous position to the current one before moving

		for (int i = 0; i < 4; i++) // Loop until the enemy moves, finds a new direction or it tries four times (to stop infinite loops)
		{
			sf::Vector2i desiredPosition = m_pos + Global::getDirectionVector(m_moveDir); // Find the desired position from the current position and direction

			// Move if not blocked, else change direction
			if (!isBlocked(desiredPosition))
			{
				move(desiredPosition);
				break; // Break from the loop if the enemy can move
			}
			else
			{
				findNewDirection();
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