/// @Author Michael Rainsford Ryan

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Basic Game" },
	m_exitGame{ false }
{
	setupShapes();

	m_tileSelector.setSize({ 32, 32 });
	m_tileSelector.setFillColor(sf::Color::Transparent);
	m_tileSelector.setOutlineColor(sf::Color::White);
	m_tileSelector.setOutlineThickness(3.0f);

	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/terrain_atlas.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	m_textureBlock.setTexture(m_tileTextures);

	if (!m_mainFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_constructionState = ConstructionMode::None;
	m_selectedTileType = TileType::None;
	m_currency = 400;
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

/// <summary>
/// Process all user events.
/// <para>Close the window when close button pressed.</para>
/// <para>Resets the maze when the reset button is pressed.</para>
/// <para>Place/remove blocks when mouse button is pressed over a tile.</para>
/// </summary>
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
		if (sf::Event::MouseMoved == nextEvent.type)
		{
			m_mousePosition = { static_cast<int>(nextEvent.mouseMove.x), static_cast<int>(nextEvent.mouseMove.y) };
			
			// convert it to world coordinates
			sf::Vector2f worldPos = m_window.mapPixelToCoords(m_mousePosition, m_gameplayView);

			m_selectedTile = static_cast<sf::Vector2i>(worldPos / TILE_SIZE);
		}
		if (sf::Event::MouseButtonPressed == nextEvent.type)
		{
			if (sf::Mouse::Left == nextEvent.mouseButton.button)
			{
				// Make sure the player doesn't remove the outer boundary
				if (m_selectedTile.x > 0 && m_selectedTile.x < MAZE_COLS - 1
					&& m_selectedTile.y > 0 && m_selectedTile.y < MAZE_ROWS - 1)
				{
					// Check if the player clicked a wall
					if (m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] != TileType::None
						&& m_constructionState == ConstructionMode::Destroying)
					{
						m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] = 0;
						m_currency += 25;
						m_constructionState = ConstructionMode::None;
					} 
					// Else the player clicked the ground. Make sure there is enough money for a wall
					else if (m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] == TileType::None && m_currency >= 30
							&& m_constructionState == ConstructionMode::Placing)
					{
						m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] = m_selectedTileType;
						m_currency -= 30;
						m_constructionState = ConstructionMode::None;
					}
				}
			}
		}

		m_gui.processEvents(nextEvent, m_constructionState, m_selectedTileType);
	}
}

/// <summary>
/// Main update function for the game.
/// <para>Updates the AI and moves them.</para>
/// <para>Updates the money string.</para>
/// </summary>
/// <param name="t_deltaTime"></param>
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

	m_gui.update(m_mousePosition, m_currency);
}

/// <summary>
/// Renders all the objects in the game.
/// <para>Renders the maze walls and AI in order of row to add overlap of walls and AI.</para>
/// <para>Draws the GUI to the screen.</para>
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	m_window.setView(m_gameplayView);

	// Draw the maze background (Grass)
	for (int row = -2; row < MAZE_ROWS + 2; row++)
	{
		for (int col = -2; col < MAZE_COLS + 2; col++)
		{
			m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE);

			m_textureBlock.setTextureRect(sf::IntRect{ 672 + (abs(row + col) % 3) * 32, 160, 32, 32 });
			m_window.draw(m_textureBlock);
		}
	}

	// Draw the tile selector
	if (m_selectedTile.y > 0 && m_selectedTile.y < MAZE_ROWS - 1 && m_selectedTile.x > 0 && m_selectedTile.x < MAZE_COLS - 1)
	{
		m_tileSelector.setPosition(m_selectedTile.x * TILE_SIZE, m_selectedTile.y * TILE_SIZE);
		m_window.draw(m_tileSelector);
	}

	// Draw the walls and AI
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			if (m_mazeBlocks[row][col] != TileType::None)
			{
				// Draw blocks red to show removing ability
				if (row == m_selectedTile.y && col == m_selectedTile.x
					&& row > 0 && row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
					&& m_constructionState == ConstructionMode::Destroying)
				{
					m_textureBlock.setColor(sf::Color{200,50,50,245});
				}

				if (m_mazeBlocks[row][col] == TileType::Slow)
				{
					m_textureBlock.setTextureRect(PLANT_TEXT_RECT);
				}
				else if (m_mazeBlocks[row][col] == TileType::Wall) {
					m_textureBlock.setTextureRect(WALL_TEXT_RECT);
				}

				m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE - 32.0f);
				m_window.draw(m_textureBlock);
				m_textureBlock.setColor(sf::Color::White);
			}
			else if (row == m_selectedTile.y && col == m_selectedTile.x
					&& row > 0 && row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
					&& m_constructionState == ConstructionMode::Placing)
			{
				if (m_selectedTileType == TileType::Slow)
				{
					m_textureBlock.setTextureRect(PLANT_TEXT_RECT);
				}
				else if (m_selectedTileType == TileType::Wall) {
					m_textureBlock.setTextureRect(WALL_TEXT_RECT);
				}

				m_textureBlock.setColor(sf::Color{ 50,100,200,180 });
				m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE - 32.0f);
				m_window.draw(m_textureBlock);
				m_textureBlock.setColor(sf::Color::White);
			}
		}

		for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
		{
			if (m_basicSolvers[i].getPos().y == row)
			{
				if (m_basicSolvers[i].getActive())
				{
					m_window.draw(m_basicSolvers[i].getBody());
				}
			}
		}
	}

	m_window.setView(m_window.getDefaultView());
	m_gui.drawScreens(m_window);
	

	m_window.display();
}

void Game::setupShapes()
{
	generateMaze();

	for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
	{
		m_basicSolvers[i].setPos(1, 0);
	}

	// Set the player's currency to 400
	m_currency = 400;
}

/// <summary>
///  Generate the maze
/// </summary>
void Game::generateMaze()
{
	// Set all the blocks to walls
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		for (int col = 0; col < MAZE_COLS; col++)
		{
			m_mazeBlocks[row][col] = TileType::Wall;
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

		// If the maze hits a dead end
		if (deadEnd)
		{
			if (!movementHistory.empty())
			{
				movementHistory.pop();
			}

			if (movementHistory.empty()) // if empty
			{
				complete = true;
				break;
			}

			row = movementHistory.top().x;
			col = movementHistory.top().y;

			deadEnd = false;
		}
		else
		{
			// Add the current position to the stack
			movementHistory.push(sf::Vector2i{ row, col });

			// Set the current tile to a ground tile
			m_mazeBlocks[row][col] = TileType::None;
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
				if (row - 2 >= 0 && m_mazeBlocks[row - 2][col] != TileType::None) {
					m_mazeBlocks[row-1][col] = TileType::None;
					row -= 2;
					foundDir = true;
				}
				upChecked = true;
				break;
			case 1:
				// Down
				if (row + 2 < MAZE_ROWS && m_mazeBlocks[row + 2][col] != TileType::None) {
					m_mazeBlocks[row+1][col] = TileType::None;
					row += 2;
					foundDir = true;
				}
				downChecked = true;
				break;
			case 2:
				// Left
				if (col - 2 >= 0 && m_mazeBlocks[row][col - 2] != TileType::None) {
					m_mazeBlocks[row][col-1] = TileType::None;
					col -= 2;
					foundDir = true;
				}
				leftChecked = true;
				break;
			case 3:
				// Right
				if (col + 2 < MAZE_COLS && m_mazeBlocks[row][col + 2] != TileType::None) {
					m_mazeBlocks[row][col+1] = TileType::None;
					col += 2;
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
