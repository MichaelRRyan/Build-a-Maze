/// @Author Michael Rainsford Ryan

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ 672u, 672u, 32u }, "Basic Game" },
	m_exitGame{ false }
{
	setupShapes();

	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/terrain_atlas.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	m_textureBlock.setTexture(m_tileTextures);
}

Game::~Game()
{

}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float FPS = 60.0f;
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // Run as many times as possible
		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // Run at a minimum of 60 fps
			update(timePerFrame); // 60 fps
		}
		render(); // Run as many times as possible
	}
}

void Game::processEvents()
{
	sf::Event nextEvent;
	while (m_window.pollEvent(nextEvent))
	{
		if (sf::Event::Closed == nextEvent.type) // check if the close window button is clicked on.
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == nextEvent.type) {
			if (sf::Keyboard::Space == nextEvent.key.code)
			{
				setupShapes();
			}
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
	{
		m_basicSolvers[i].move(m_mazeBlocks);
	}
}

void Game::render()
{
	m_window.clear();

	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE);

			m_textureBlock.setTextureRect(sf::IntRect{ 672 + ((row + col) % 3) * 32, 160, 32, 32 });
			m_window.draw(m_textureBlock);
		}
	}

	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			if (m_mazeBlocks[row][col] == 10)
			{
				m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				m_textureBlock.setTextureRect(sf::IntRect{ 0, 832, 32, 64 });
				m_textureBlock.move(0.0f, -32.0f);
				m_window.draw(m_textureBlock);
			}
		}

		for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
		{
			if (m_basicSolvers[i].getPos().y == row)
			{
				m_window.draw(m_basicSolvers[i].getBody());
			}
		}
	}

	m_window.display();
}

void Game::setupShapes()
{
	generateMaze();

	for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
	{
		m_basicSolvers[i].setPos(1, 0);
	}
}

/// <summary>
///  Generate the maze
/// </summary>
void Game::generateMaze()
{
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			m_mazeBlocks[row][col] = 10;
		}
	}

	// Create a stack to store the history of maze movements
	std::stack <sf::Vector2i> movementHistory;
	
	// Start the maze at the top left corner
	int row = 1;
	int col = 1;

	// Create the direction checked bools
	bool leftChecked, rightChecked, upChecked, downChecked;

	bool deadEnd = false;

	// Set complete to false and start looping
	bool complete = false;
	while (!complete) {
		if (deadEnd)
		{
			if (!movementHistory.empty())
			{
				movementHistory.pop();

				if (movementHistory.empty())
				{
					complete = true;
					break;
				}
			}
			else
			{
				complete = true;
				break;
			}
			row = movementHistory.top().y;
			col = movementHistory.top().x;

			deadEnd = false;
		}
		else
		{
			// Add the current position to the stack
			movementHistory.push(sf::Vector2i{ row, col });

			// Set the current tile to a ground tile
			m_mazeBlocks[row][col] = 0;
		}

		// Set all direction checks to false
		leftChecked = false;
		rightChecked = false;
		upChecked = false;
		downChecked = false;

		// Create a foundDir bool and start looping
		bool foundDir = false;
		while (!foundDir)
		{
			// If all directions have been checked, break out
			if (leftChecked && rightChecked && upChecked && downChecked) {
				deadEnd = true;
				break;
			}

			// Choose a random direction
			int dir = rand() % 4;

			// Check the direction has not already been checked, if it has, choose a new direction
			if (dir == 0 && upChecked) {
				continue;
			}
			if (dir == 1 && downChecked) {
				continue;
			}
			if (dir == 2 && leftChecked) {
				continue;
			}
			if (dir == 3 && rightChecked) {
				continue;
			}


			switch (dir) {
			case 0:
				// Up
				if (row - 2 >= 0 && m_mazeBlocks[row - 2][col] != 0) {
					row--;
					m_mazeBlocks[row][col] = 0;
					row--;
					foundDir = true;
				}
				upChecked = true;
				break;
			case 1:
				// Down
				if (row + 2 < MAZE_ROWS && m_mazeBlocks[row + 2][col] != 0) {
					row++;
					m_mazeBlocks[row][col] = 0;
					row++;
					foundDir = true;
				}
				downChecked = true;
				break;
			case 2:
				// Left
				if (col - 2 >= 0 && m_mazeBlocks[row][col - 2] != 0) {
					col--;
					m_mazeBlocks[row][col] = 0;
					col--;
					foundDir = true;
				}
				leftChecked = true;
				break;
			case 3:
				// Right
				if (col + 2 < MAZE_COLS && m_mazeBlocks[row][col + 2] != 0) {
					col++;
					m_mazeBlocks[row][col] = 0;
					col++;
					foundDir = true;
				}
				rightChecked = true;
				break;
			}
		}
	}

	// Clear the maze entrance and finish
	m_mazeBlocks[1][0] = 0;
	m_mazeBlocks[MAZE_ROWS - 2][MAZE_COLS - 1] = 0;
}
