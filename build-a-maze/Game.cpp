/// @Author Michael Rainsford Ryan

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Basic Game" },
	m_exitGame{ false },
	m_gamestate{ GameState::BuildMode }, // Set the start game state to 'BuildMode'
	m_timeModifier{ 1 }
{
	setupShapes();
	setupGame();
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
#ifdef _DEBUG
			if (sf::Keyboard::R == nextEvent.key.code)
			{
				setupShapes();
			}
#endif // _DEBUG
			if (sf::Keyboard::Space == nextEvent.key.code)
			{
				if (m_gamestate == GameState::BuildMode)
				{
					m_gamestate = GameState::Simulation;

					// Reset all AI's positions to the start of the maze
					for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
					{
						m_basicSolvers[i].setPos(1, 0);
						m_basicSolvers[i].setActive(true);
						m_basicSolvers[i].setMoveTimer(i * 60);
						m_basicSolvers[i].setCharacterDirection(-100); // TEMP: Sloppy fix but works for now
					}

					m_prevTimeToComplete = 0;
					m_timeToComplete = 0;
					m_moneyEarned = 0;
				}
				else if (m_gamestate == GameState::Simulation)
				{
					m_gamestate = GameState::BuildMode;
				}
			}
			if (sf::Keyboard::Num1 == nextEvent.key.code)
			{
				if (m_timeModifier > 0.25)
				{
					m_timeModifier *= 0.5f;

					for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
					{
						m_basicSolvers[i].setTimeModifier(m_timeModifier);
					}
					std::cout << "Time set to " << m_timeModifier << std::endl;
				}
				else
				{
					std::cout << "Time already maximum speed" << std::endl;
				}
			}
			else if (sf::Keyboard::Num2 == nextEvent.key.code)
			{
				m_timeModifier = 1;
				for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
				{
					m_basicSolvers[i].setTimeModifier(1);
				}
				std::cout << "Time set to 1" << std::endl;
			}
			else if (sf::Keyboard::Num3 == nextEvent.key.code)
			{
				if (m_timeModifier < 4)
				{
					m_timeModifier *= 2.0f;

					for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
					{
						m_basicSolvers[i].setTimeModifier(m_timeModifier);
					}
					std::cout << "Time set to " << m_timeModifier << std::endl;
				}
				else
				{
					std::cout << "Time already minimum speed" << std::endl;
				}
			}
		}
		if (sf::Event::MouseMoved == nextEvent.type)
		{
			m_mousePosition = { static_cast<int>(nextEvent.mouseMove.x), static_cast<int>(nextEvent.mouseMove.y) };

			// convert it to world coordinates
			sf::Vector2f worldPos = m_window.mapPixelToCoords(m_mousePosition, m_gameplayView);

			m_selectedTile = static_cast<sf::Vector2i>(worldPos / TILE_SIZE);
		}
		if (m_gamestate == GameState::BuildMode)
		{
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

	if (m_gamestate == GameState::Simulation)
	{
		int noOfAI = 0;

		for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
		{
			if (m_basicSolvers[i].getActive())
			{
				m_basicSolvers[i].move(m_mazeBlocks);
				noOfAI++;
			}
		}

		if (noOfAI > 0)
		{
			m_timeToComplete += t_deltaTime.asSeconds() / m_timeModifier;
		}
		else
		{
			m_currency += m_moneyEarned;
			m_gamestate = GameState::BuildMode;
		}

		if (static_cast<int>(floor(m_prevTimeToComplete)) < static_cast<int>(floor(m_timeToComplete)))
		{
			m_moneyEarned += noOfAI;
			m_prevTimeToComplete = m_timeToComplete;
		}

		// Work out minutes and seconds and set the string
		int seconds = static_cast<int>(floor(m_timeToComplete)) % 60;
		int minutes = static_cast<int>(floor(m_timeToComplete)) / 60;

		if (seconds < 10) // if below 10 seconds, add a zero before the seconds as to display correctly as a time
		{
			m_aiInfoText.setString(std::to_string(noOfAI) + "/" + std::to_string(BASIC_SOLVERS_MAX) + "Guests left in the maze\nTime: " + std::to_string(minutes) + ":0" + std::to_string(seconds) + "\nMoney earned: " + std::to_string(m_moneyEarned));
		}
		else
		{
			m_aiInfoText.setString(std::to_string(noOfAI) + "/" + std::to_string(BASIC_SOLVERS_MAX) + "Guests left in the maze\nTime: " + std::to_string(minutes) + ":" + std::to_string(seconds) + "\nMoney earned: " + std::to_string(m_moneyEarned));
		}
	}

	if (m_gamestate == GameState::BuildMode)
	{
		m_gui.update(m_mousePosition, m_currency);
	}
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
					&& m_constructionState == ConstructionMode::Destroying
					&& m_gamestate == GameState::BuildMode)
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
			// Draw ghost blocks when placing a tile
			else if (row == m_selectedTile.y && col == m_selectedTile.x
					&& row > 0 && row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
					&& m_constructionState == ConstructionMode::Placing
					&& m_gamestate == GameState::BuildMode)
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

		// Draw the AI if in simulation mode
		if (m_gamestate == GameState::Simulation)
		{
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
	}

	m_window.setView(m_window.getDefaultView());
	m_gui.drawScreens(m_window);
	
	if (m_gamestate == GameState::Simulation)
	{
		m_window.draw(m_aiInfoText);
	}

	m_window.display();
}

/// <summary>
/// Sets up the game
/// </summary>
void Game::setupGame()
{
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

	m_aiInfoText.setFont(m_mainFont);
	m_aiInfoText.setFillColor(sf::Color::Black);
	m_aiInfoText.setStyle(sf::Text::Style::Bold);
	m_aiInfoText.setPosition(200.0f, 20.0f);
	m_aiInfoText.setOutlineColor(sf::Color{ 225,225,225 });
	m_aiInfoText.setOutlineThickness(2.5f);

	m_constructionState = ConstructionMode::None;
	m_selectedTileType = TileType::None;
	m_currency = 400;
}

void Game::setupShapes()
{
	generateMaze();

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
