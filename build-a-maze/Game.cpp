/// @Author Michael Rainsford Ryan

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Build-a-Maze!" },
	m_exitGame{ false },
	m_gamestate{ GameState::TitleScreen }, // Set the start game state to 'Title screen'
	m_timeModifier{ 1.0f },
	m_gameplayView{ { 240.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_constructionView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	//m_constructionView{ { 150.0f, 100.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.25f, static_cast<float>(WINDOW_HEIGHT) * 0.25f} },
	m_controllerSensitivity{ 0.25f },
	m_cursor{ 0 }
{
	MazeGenerator::generateMaze(m_mazeBlocks);
	setupGame();

	// Set the player's currency to 400
	m_currency = 400;

	m_controllerConnected = m_controller.connect();
}

Game::~Game()
{
	for (MazeSolver* solver : m_mazeSolverPtrs)
	{
		delete solver;
	}
	m_mazeSolverPtrs.clear();
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
			render(); // Run as many times as possible
		}
		
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
		processKeyboardEvents(nextEvent);
		processMouseEvents(nextEvent);

		if (m_gamestate == GameState::TitleScreen)
		{
			m_gui.processTitleEvents(m_cursor, m_gamestate, m_exitGame);
		}

		m_gui.processEvents(nextEvent, m_cursor, m_constructionState, m_selectedTileType);

	} // End while poll event
}

/// <summary>
/// Processes user keyboard events in the game
/// </summary>
/// <param name="t_event">Current Event</param>
void Game::processKeyboardEvents(sf::Event t_event)
{
	if (sf::Event::KeyPressed == t_event.type)
	{
		if (sf::Keyboard::Space == t_event.key.code)
		{
			if (m_gamestate == GameState::BuildMode)
			{
				if (m_simDetailsDisplay)
				{
					m_simDetailsDisplay = false;
				}
				else
				{
					m_gamestate = GameState::Simulation;
					resetSimulation();
				}
			}
			else if (m_gamestate == GameState::Simulation)
			{
				m_gamestate = GameState::BuildMode;
				m_gamePaused = false;
			}
		}

		// Pause the game
		if (sf::Keyboard::P == t_event.key.code)
		{
			if (m_gamestate == GameState::Simulation)
			{
				m_gamePaused = !m_gamePaused;
			}

		}

		processTimeModifierEvents(t_event);

		if (sf::Keyboard::Escape == t_event.key.code)
		{
			if (m_gamestate == GameState::BuildMode
				&& m_constructionState == ConstructionMode::None)
			{
				m_gamestate = GameState::TitleScreen;
			}
		}

		if (sf::Keyboard::R == t_event.key.code)
		{
			if (m_gamestate == GameState::BuildMode
				&& m_constructionState == ConstructionMode::Placing)
			{
				switch (m_selectedTileType)
				{
				case TileType::Treadmill_left:
					m_selectedTileType = TileType::Treadmill_up;
					break;
				case TileType::Treadmill_right:
					m_selectedTileType = TileType::Treadmill_down;
					break;
				case TileType::Treadmill_up:
					m_selectedTileType = TileType::Treadmill_right;
					break;
				case TileType::Treadmill_down:
					m_selectedTileType = TileType::Treadmill_left;
					break;
				}
				
			}
		}
	}
}

/// <summary>
/// Processes the user mouse events for the game
/// </summary>
/// <param name="t_event">Current event</param>
void Game::processMouseEvents(sf::Event t_event)
{
	if (sf::Event::MouseMoved == t_event.type)
	{
		m_cursor.m_position = { static_cast<int>(t_event.mouseMove.x), static_cast<int>(t_event.mouseMove.y) };
	}

	if (m_cursor.m_clicked)
	{
		if (m_gamestate == GameState::BuildMode)
		{
			// Make sure the player doesn't remove the outer boundary
			if (m_selectedTile.x > 0 && m_selectedTile.x < MAZE_COLS - 1
				&& m_selectedTile.y > 0 && m_selectedTile.y < MAZE_ROWS - 1)
			{
				// Check if the player clicked a wall
				if (m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] != TileType::None
					&& m_constructionState == ConstructionMode::Destroying)
				{
					m_mazeBlocks[m_selectedTile.y][m_selectedTile.x] = TileType::None;
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

	if (!m_gamePaused)
	{
		// If the game state is simulation
		if (m_gamestate == GameState::Simulation)
		{
			// Reset AI count
			m_noOfAI = 0;

			// Loop through and update all AI. Count those that are still active
			for (MazeSolver* solver : m_mazeSolverPtrs)
			{
				if (solver->getActive())
				{
					solver->update(m_mazeBlocks);
					m_noOfAI++;
				}
			}
			
			// If there are AI in the maze count the time
			if (m_noOfAI > 0)
			{
				m_timeToComplete += t_deltaTime.asSeconds() / m_timeModifier;
			}
			// Else add the money earned to currency and switch modes
			else
			{
				m_currency += m_moneyEarned;
				m_gamestate = GameState::BuildMode;
				m_simDetailsDisplay = true;
			}

			// Update the money earned from a sim each second
			if (static_cast<int>(floor(m_prevTimeToComplete)) < static_cast<int>(floor(m_timeToComplete)))
			{
				m_moneyEarned += m_noOfAI;
				m_prevTimeToComplete = m_timeToComplete;
			}
		}
	}

	// Update the build mode GUI
	if (m_gamestate == GameState::BuildMode)
	{
		m_gui.update(m_cursor.m_position, m_currency);
	}

	updateCursor();
}

/// <summary>
/// Renders all the objects in the game.
/// <para>Renders the maze walls and AI in order of row to add overlap of walls and AI.</para>
/// <para>Draws the GUI to the screen.</para>
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	if (m_gamestate == GameState::BuildMode && !m_simDetailsDisplay) {
		m_window.setView(m_constructionView);
	}
	else
	{
		m_window.setView(m_gameplayView);
	}

	Renderer::drawMazeBackground(m_window, m_textureBlock);

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
			if (static_cast<TileType>(m_mazeBlocks[row][col]) != TileType::None)
			{
				// Draw blocks red to show removing ability
				if (row == m_selectedTile.y && col == m_selectedTile.x
					&& row > 0 && row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
					&& m_constructionState == ConstructionMode::Destroying
					&& m_gamestate == GameState::BuildMode)
				{
					m_textureBlock.setColor(sf::Color{ 200,50,50,245 });
				}

				m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE - 32.0f);

				if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Slow)
				{
					m_textureBlock.setTextureRect(PLANT_TEXT_RECT);
				}
				else if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Wall) {
					m_textureBlock.setTextureRect(WALL_TEXT_RECT);
				}
				else if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Treadmill_left) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 0, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Treadmill_right) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 32, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Treadmill_up) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 64, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (static_cast<TileType>(m_mazeBlocks[row][col]) == TileType::Treadmill_down) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 96, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}

				m_window.draw(m_textureBlock);
				m_textureBlock.setColor(sf::Color::White);
			}
			// Draw ghost blocks when placing a tile
			else if (row == m_selectedTile.y && col == m_selectedTile.x
				&& row > 0 && row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
				&& m_constructionState == ConstructionMode::Placing
				&& m_gamestate == GameState::BuildMode)
			{
				m_textureBlock.setPosition(col * TILE_SIZE, row * TILE_SIZE - 32.0f);

				if (m_selectedTileType == TileType::Slow)
				{
					m_textureBlock.setTextureRect(PLANT_TEXT_RECT);
				}
				else if (m_selectedTileType == TileType::Wall) {
					m_textureBlock.setTextureRect(WALL_TEXT_RECT);
				}
				else if (m_selectedTileType == TileType::Treadmill_left) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 0, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (m_selectedTileType == TileType::Treadmill_right) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 32, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (m_selectedTileType == TileType::Treadmill_up) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 64, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}
				else if (m_selectedTileType == TileType::Treadmill_down) {
					int left = 32 * (static_cast<int>(m_treadmillAnimClock.getElapsedTime().asMilliseconds()) / 2 % 4);
					m_textureBlock.setTextureRect({ 32 + left, 96, 32, 32 });
					m_textureBlock.move(0.0f, 32.0f);
				}

				m_textureBlock.setColor(sf::Color{ 50,100,200,180 });
				m_window.draw(m_textureBlock);
				m_textureBlock.setColor(sf::Color::White);
			}
		}

		// Draw the AI if in simulation mode
		if (m_gamestate == GameState::Simulation)
		{
			for (MazeSolver* solver : m_mazeSolverPtrs)
			{
				if (solver->getActive())
				{
					if (solver->getPos().y == row)
					{
						solver->draw(m_window);
					}
				}
			}
		}
	}

	// Draws GUI
	m_window.setView(m_window.getDefaultView());
	if (m_gamestate == GameState::TitleScreen)
	{
		m_gui.drawTitleScreen(m_window);
	}
	else if (m_gamestate == GameState::BuildMode && !m_simDetailsDisplay)
	{
		m_gui.drawConstructionGUI(m_window);
	}
	else
	{
		m_gui.drawSimulationGUI(m_window, m_noOfAI, m_timeToComplete, m_moneyEarned);

		if (m_gamePaused)
		{
			m_window.draw(m_pauseScreenFade);
			m_window.draw(m_pauseText);
		}
	}

	m_cursor.m_sprite.setPosition(static_cast<sf::Vector2f>(m_cursor.m_position));
	m_window.draw(m_cursor.m_sprite);

	m_window.display();
}

/// <summary>
/// Sets up the game
/// </summary>
void Game::setupGame()
{
	m_window.setMouseCursorVisible(false);

	m_tileSelector.setSize({ 32, 32 });
	m_tileSelector.setFillColor(sf::Color::Transparent);
	m_tileSelector.setOutlineColor(sf::Color::White);
	m_tileSelector.setOutlineThickness(3.0f);

	m_pauseScreenFade.setSize({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
	m_pauseScreenFade.setFillColor(sf::Color{ 255, 255, 255, 100 });

	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	if (!m_cursor.m_texture.loadFromFile("ASSETS/IMAGES/Cursors.png"))
	{
		std::cout << "Error loading cursor texture";
	}

	m_textureBlock.setTexture(m_tileTextures);
	m_cursor.m_sprite.setTexture(m_cursor.m_texture);
	m_cursor.m_sprite.setTextureRect(sf::IntRect{ { 0,0 }, { 64,64 } });
	m_cursor.m_sprite.setOrigin(12.0f, 0.0f);

	if (!m_mainFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_pauseText.setString("PAUSED");
	m_pauseText.setCharacterSize(100u);
	m_pauseText.setFont(m_mainFont);
	m_pauseText.setFillColor(sf::Color{0, 0, 0, 150});
	m_pauseText.setPosition(WINDOW_WIDTH / 2, static_cast<float>(WINDOW_HEIGHT) / 2.5f);
	m_pauseText.setOrigin(m_pauseText.getGlobalBounds().width / 2, m_pauseText.getGlobalBounds().height / 2);

	m_constructionState = ConstructionMode::None;
	m_selectedTileType = TileType::None;
	m_currency = 400;
	m_simDetailsDisplay = false;
	m_gamePaused = false;

	// Add a random selection of AI to the maze
	m_mazeSolverPtrs.clear(); // Clear any previous AI
	for (int i = 0; i < SOLVERS_MAX; i++)
	{
		switch (rand() % 3)
		{
		case 0:
			m_mazeSolverPtrs.push_back(new BasicSolver);
			break;

		case 1:
			m_mazeSolverPtrs.push_back(new Mathematician);
			break;

		case 2:
			m_mazeSolverPtrs.push_back(new Cartographer);
			break;
		}
	}
}

/// <summary>
/// @brief resets all the game data used for simulation
/// </summary>
void Game::resetSimulation()
{
	m_constructionState = ConstructionMode::None;
	m_prevTimeToComplete = 0;
	m_timeToComplete = 0;
	m_moneyEarned = 0;
	m_timeModifier = 1.0f;

	// Reset all AI's positions to the start of the maze
	int moveDelayCounter = 0; // Counts the number of maze solvers and uses this to set the delay on movement
	for (MazeSolver* solver : m_mazeSolverPtrs)
	{
		solver->reset(moveDelayCounter * 60);
		moveDelayCounter++;
	}

	std::cout << "Time set to 1" << std::endl;
}

/// <summary>
/// @brief process events related to the time modifier
/// </summary>
/// <param name="t_event">user event</param>
void Game::processTimeModifierEvents(sf::Event t_event)
{
	if (sf::Keyboard::Num1 == t_event.key.code)
	{
		if (m_timeModifier > 0.25)
		{
			m_timeModifier *= 0.5f;

			for (MazeSolver* solver : m_mazeSolverPtrs)
			{
				solver->setTimeModifier(m_timeModifier);
			}

			std::cout << "Time set to " << m_timeModifier << std::endl;
		}
		else
		{
			std::cout << "Time already maximum speed" << std::endl;
		}
	}
	else if (sf::Keyboard::Num2 == t_event.key.code)
	{
		m_timeModifier = 1;

		for (MazeSolver* solver : m_mazeSolverPtrs)
		{
			solver->setTimeModifier(m_timeModifier);
		}

		std::cout << "Time set to 1" << std::endl;
	}
	else if (sf::Keyboard::Num3 == t_event.key.code)
	{
		if (m_timeModifier < 4)
		{
			m_timeModifier *= 2.0f;

			for (MazeSolver* solver : m_mazeSolverPtrs)
			{
				solver->setTimeModifier(m_timeModifier);
			}

			std::cout << "Time set to " << m_timeModifier << std::endl;
		}
		else
		{
			std::cout << "Time already minimum speed" << std::endl;
		}
	}
}

/// <summary>
/// @brief update the cursor
/// </summary>
void Game::updateCursor()
{
	// Keep the cursor image up to date
	if (m_constructionState == ConstructionMode::Placing)
	{
		m_cursor.m_sprite.setTextureRect(sf::IntRect{ { 128, 0 }, { 64, 64 } });
	}
	else if (m_constructionState == ConstructionMode::Destroying)
	{
		m_cursor.m_sprite.setTextureRect(sf::IntRect{ { 64, 0 }, { 64, 64 } });
	}
	else
	{
		m_cursor.m_sprite.setTextureRect(sf::IntRect{ { 0, 0 }, { 64, 64 } });
	}

	// Reset the cursor click
	m_cursor.m_clicked = false;
	m_cursor.m_cancelClicked = false;

	// Check if the mouse has been pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_cursor.m_clicked = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_cursor.m_cancelClicked = true;
	}

	// Update the controller if connected
	if (m_controllerConnected)
	{
		updateController();
	}

	// convert the cursor position to world coordinates
	sf::Vector2f worldPos;
	if (m_gamestate == GameState::BuildMode && !m_simDetailsDisplay)
	{
		worldPos = m_window.mapPixelToCoords(m_cursor.m_position, m_constructionView);
	}
	else
	{
		worldPos = m_window.mapPixelToCoords(m_cursor.m_position, m_gameplayView);
	}

	m_selectedTile = static_cast<sf::Vector2i>(worldPos / TILE_SIZE);
}

/// <summary>
/// @brief Update the controller and take input
/// </summary>
void Game::updateController()
{
	m_controller.update();

	// Controller Right
	if (m_controller.currentState.LeftThumbStick.x > 15.0f)
	{
		if (m_cursor.m_position.x < WINDOW_WIDTH - m_cursor.m_sprite.getGlobalBounds().width / 2.0f)
		{
			m_cursor.m_position.x += static_cast<int>(m_controller.currentState.LeftThumbStick.x * m_controllerSensitivity);
		}
		else
		{
			m_cursor.m_position.x = WINDOW_WIDTH - static_cast<int>(m_cursor.m_sprite.getGlobalBounds().width / 2.0f);
		}
	}

	// Controller Left
	if (m_controller.currentState.LeftThumbStick.x < -15.0f)
	{
		if (m_cursor.m_position.x > 0.0f)
		{
			m_cursor.m_position.x += static_cast<int>(m_controller.currentState.LeftThumbStick.x * m_controllerSensitivity);
		}
		else
		{
			m_cursor.m_position.x = 0;
		}
	}

	// Controller Down
	if (m_controller.currentState.LeftThumbStick.y > 15.0f)
	{
		if (m_cursor.m_position.y < WINDOW_HEIGHT - m_cursor.m_sprite.getGlobalBounds().height / 2.0f)
		{
			m_cursor.m_position.y += static_cast<int>(m_controller.currentState.LeftThumbStick.y * m_controllerSensitivity);
		}
		else
		{
			m_cursor.m_position.y = WINDOW_HEIGHT - static_cast<int>(m_cursor.m_sprite.getGlobalBounds().height / 2.0f);
		}
	}

	// Controller Up
	if (m_controller.currentState.LeftThumbStick.y < -15.0f)
	{
		if (m_cursor.m_position.y > 0.0f)
		{
			m_cursor.m_position.y += static_cast<int>(m_controller.currentState.LeftThumbStick.y * m_controllerSensitivity);
		}
		else
		{
			m_cursor.m_position.y = 0;
		}
	}

	// Check if either the controller has been clicked
	if (m_controller.currentState.A)
	{
		m_cursor.m_clicked = true;
	}
	if (m_controller.currentState.B)
	{
		m_cursor.m_cancelClicked = true;
	}
}