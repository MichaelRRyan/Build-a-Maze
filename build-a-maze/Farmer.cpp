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
	m_destroying = false;
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
		m_destroying = false;

		// Check for new pathways on all sides
		sf::Vector2i dir = Global::getDirectionVector(m_moveDir);
		m_previousMoveDir = m_moveDir;

		// Positive
		if (!isBlocked({ m_pos.x + dir.y, m_pos.y + dir.x }))
		{
			if (rand() % 5 == 0) {
				m_moveDir = Global::getDirection({ dir.y, dir.x });
			}
		}

		// Negative
		if (!isBlocked({ m_pos.x - dir.y, m_pos.y - dir.x }))
		{
			if (rand() % 5 == 0) {
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
			// Random chance to destroy a wall if blocked
			else if (rand() % 5 == 0 // 1 in 5 chance
				&& desiredPosition.x > 0 && desiredPosition.x < MAZE_SIZE - 1 // Boundary checking
				&& desiredPosition.y > 0 && desiredPosition.y < MAZE_SIZE - 1 // Boundary checking
				&& m_mazeRef[desiredPosition.y][desiredPosition.x] == TileType::Wall) // Check if wall
			{
				m_mazeRef[desiredPosition.y][desiredPosition.x].setType(TileType::None);
				m_destroying = true;
				break;
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

void Farmer::animate()
{
	// Work out the new X and Y with Linear Interpolation
	float newX = (m_pos.x * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.x * 32) * (1.0f * m_moveTimer / m_movementSpeed);
	float newY = (m_pos.y * 32) * (1.0f - (1.0f * m_moveTimer / m_movementSpeed)) + (m_previousPos.y * 32) * (1.0f * m_moveTimer / m_movementSpeed);

	// Set the position to the current cell
	m_body.setPosition(static_cast<float>(newX), static_cast<float>(newY));

	int frameNum = static_cast<int>((1.0 * m_moveTimer / m_movementSpeed) * 3);

	if (m_destroying)
	{
		sf::IntRect frame = sf::IntRect{ m_characterNumber.x + 48 + (16 * frameNum), m_characterNumber.y + m_characterDirection * 32, 16, 32 }; // Character height = 64, character width = 32

		m_body.setTextureRect(frame);
	}
	else
	{
		sf::IntRect frame = sf::IntRect{ m_characterNumber.x + (16 * frameNum), m_characterNumber.y + m_characterDirection * 32, 16, 32 }; // Character height = 64, character width = 32

		// Play footstep if just changed to middle frame
		if (frame.left == 16 && m_body.getTextureRect().left != 16)
		{
			playFootstep();
		}

		m_body.setTextureRect(frame);
	}
}
