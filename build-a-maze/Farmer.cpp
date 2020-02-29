#include "Farmer.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 29/02/2020
/// </summary>

Farmer::Farmer(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_maze, std::vector<Sheep*>& t_sheepRef) :
	MazeSolver(t_maze, t_sheepRef)
{
	loadFiles();
	m_moveDir = Direction::East;
	m_moveTimer = 0;
	m_active = true;
	m_movementSpeed = DEFAULT_MOVE_SPEED;
	m_timeModifier = 1.0f;
}

void Farmer::loadFiles()
{
	m_characterNumber = { 0, 96 * 3 };

	if (!m_spriteSheet.loadFromFile("ASSETS\\IMAGES\\character_sheet.png"))
	{
		// Error loading image
	}

	m_body.setTexture(m_spriteSheet); // Set the character texture
	m_body.setTextureRect(sf::IntRect{ m_characterNumber.x, m_characterNumber.y, 16, 32 }); // Set the character
	m_body.setOrigin(0.0f, 16.0f); // Set the origin of the sprite to ignore the head part of the sprite
}

void Farmer::update()
{
	checkSheepCollisions();

	if (m_moveTimer <= 0) // The enemy can only move once its timer reaches zero
	{
		// Check for new pathways on all sides
		sf::Vector2i dir = Global::getDirectionVector(m_moveDir);
		m_previousMoveDir = m_moveDir;

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

		checkForExit(); // Check for the exit nearby
		checkForSheep(); // Check for sheep nearby

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

		checkIfOutOfMaze(); // Check if the solver has gotten out of the maze
	}
	else
	{
		m_moveTimer--;
		animate();
	}
}