/// @Author Michael Rainsford Ryan

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Build-a-Maze!" },
	m_exitGame{ false },
	m_gamestate{ GameState::TitleScreen }, // Set the start game state to 'BuildMode'
	m_timeModifier{ 1.0f },
	m_gameplayView{ { 240.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_constructionView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} }
{
	setupShapes();
	setupGame();
	m_controllerConnected = m_controller.connect();
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
		processKeyboardEvents(nextEvent);
		processMouseEvents(nextEvent);

		if (m_gamestate == GameState::TitleScreen)
		{
			m_gui.processTitleEvents(nextEvent, m_gamestate, m_exitGame);
		}

		m_gui.processEvents(nextEvent, m_mousePosition, m_controller, m_constructionState, m_selectedTileType);

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

					// Reset all AI's positions to the start of the maze
					for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
					{
						m_basicSolvers[i].setPos(1, 0);
						m_basicSolvers[i].setActive(true);
						m_basicSolvers[i].setMoveTimer(i * 60);
						m_basicSolvers[i].setCharacterDirection(-100); // TEMP: Sloppy fix but works for now
					}


					m_mathematician.setPos(1, 0);
					m_mathematician.setActive(true);
					m_mathematician.setMoveTimer(BASIC_SOLVERS_MAX * 60);
					m_mathematician.setCharacterDirection(-100); // TEMP: Sloppy fix but works for now

					m_constructionState = ConstructionMode::None;
					m_prevTimeToComplete = 0;
					m_timeToComplete = 0;
					m_moneyEarned = 0;
					m_timeModifier = 1.0f;
					for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
					{
						m_basicSolvers[i].setTimeModifier(1);
					}

					m_mathematician.setTimeModifier(1);

					std::cout << "Time set to 1" << std::endl;
				}
			}
			else if (m_gamestate == GameState::Simulation)
			{
				m_gamestate = GameState::BuildMode;
				m_gamePaused = false;
			}
		}

		if (sf::Keyboard::P == t_event.key.code)
		{
			if (m_gamestate == GameState::Simulation)
			{
				m_gamePaused = !m_gamePaused;
			}

		}

		if (sf::Keyboard::Num1 == t_event.key.code)
		{
			if (m_timeModifier > 0.25)
			{
				m_timeModifier *= 0.5f;

				for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
				{
					m_basicSolvers[i].setTimeModifier(m_timeModifier);
				}

				m_mathematician.setTimeModifier(m_timeModifier);

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
			for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
			{
				m_basicSolvers[i].setTimeModifier(1);
			}

			m_mathematician.setTimeModifier(1);
			std::cout << "Time set to 1" << std::endl;
		}
		else if (sf::Keyboard::Num3 == t_event.key.code)
		{
			if (m_timeModifier < 4)
			{
				m_timeModifier *= 2.0f;

				for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
				{
					m_basicSolvers[i].setTimeModifier(m_timeModifier);
				}

				m_mathematician.setTimeModifier(m_timeModifier);
				std::cout << "Time set to " << m_timeModifier << std::endl;
			}
			else
			{
				std::cout << "Time already minimum speed" << std::endl;
			}
		}

		if (sf::Keyboard::Escape == t_event.key.code)
		{
			if (m_gamestate == GameState::BuildMode)
			{
				if (m_constructionState == ConstructionMode::None)
				{
					m_gamestate = GameState::TitleScreen;
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
		m_mousePosition = { static_cast<int>(t_event.mouseMove.x), static_cast<int>(t_event.mouseMove.y) };

		// convert it to world coordinates
		sf::Vector2f worldPos;

		if (m_gamestate == GameState::BuildMode && !m_simDetailsDisplay)
		{
			worldPos = m_window.mapPixelToCoords(m_mousePosition, m_constructionView);
		}
		else
		{
			worldPos = m_window.mapPixelToCoords(m_mousePosition, m_gameplayView);
		}

		m_selectedTile = static_cast<sf::Vector2i>(worldPos / TILE_SIZE);
	}
	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		if (sf::Mouse::Left == t_event.mouseButton.button)
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
			for (int i = 0; i < BASIC_SOLVERS_MAX; i++)
			{
				if (m_basicSolvers[i].getActive())
				{
					m_basicSolvers[i].move(m_mazeBlocks);
					m_noOfAI++;
				}
			}

			if (m_mathematician.getActive())
			{
				m_mathematician.move(m_mazeBlocks);
				m_noOfAI++;
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
		m_gui.update(m_mousePosition, m_currency);
	}

	if (m_constructionState == ConstructionMode::Placing)
	{
		m_cursor.setTextureRect(sf::IntRect{ { 128, 0 }, { 64, 64 } });
	}
	else if (m_constructionState == ConstructionMode::Destroying)
	{
		m_cursor.setTextureRect(sf::IntRect{ { 64, 0 }, { 64, 64 } });
	}
	else
	{
		m_cursor.setTextureRect(sf::IntRect{ { 0, 0 }, { 64, 64 } });
	}

	// Update the controller if connected
	if (m_controllerConnected)
	{
		updateController();
	}
}

/// <summary>
/// Update the controller and take input
/// </summary>
void Game::updateController()
{
	m_controller.update();

	// Controller Right
	if (m_controller.currentState.LeftThumbStick.x > 15.0f)
	{
		if (m_mousePosition.x < WINDOW_WIDTH - m_cursor.getGlobalBounds().width / 2.0f)
		{
			m_mousePosition.x += m_controller.currentState.LeftThumbStick.x / 2.0f;
		}
		else
		{
			m_mousePosition.x = WINDOW_WIDTH - m_cursor.getGlobalBounds().width / 2.0f;
		}
	}

	// Controller Left
	if (m_controller.currentState.LeftThumbStick.x < -15.0f)
	{
		if (m_mousePosition.x > 0.0f)
		{
			m_mousePosition.x += m_controller.currentState.LeftThumbStick.x / 2.0f;
		}
		else
		{
			m_mousePosition.x = 0.0f;
		}
	}

	// Controller Down
	if (m_controller.currentState.LeftThumbStick.y > 15.0f)
	{
		if (m_mousePosition.y < WINDOW_HEIGHT - m_cursor.getGlobalBounds().height / 2.0f)
		{
			m_mousePosition.y += m_controller.currentState.LeftThumbStick.y / 2.0f;
		}
		else
		{
			m_mousePosition.y = WINDOW_HEIGHT - m_cursor.getGlobalBounds().height / 2.0f;
		}
	}

	// Controller Up
	if (m_controller.currentState.LeftThumbStick.y < -15.0f)
	{
		if (m_mousePosition.y > 0.0f)
		{
			m_mousePosition.y += m_controller.currentState.LeftThumbStick.y / 2.0f;
		}
		else
		{
			m_mousePosition.y = 0.0f;
		}
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

	if (m_gamestate == GameState::BuildMode && !m_simDetailsDisplay) {
		m_window.setView(m_constructionView);
	}
	else
	{
		m_window.setView(m_gameplayView);
	}

	// Draw the maze background (Grass)
	for (int row = -2; row < MAZE_ROWS + 2; row++)
	{
		for (int col = -8; col < MAZE_COLS + 2; col++)
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


			if (m_mathematician.getPos().y == row)
			{
				if (m_mathematician.getActive())
				{
					m_window.draw(m_mathematician.getBody());
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

	m_cursor.setPosition(static_cast<sf::Vector2f>(m_mousePosition));
	m_window.draw(m_cursor);

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

	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/terrain_atlas.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	if (!m_cursorTexture.loadFromFile("ASSETS/IMAGES/Cursors.png"))
	{
		std::cout << "Error loading cursor texture";
	}

	m_textureBlock.setTexture(m_tileTextures);
	m_cursor.setTexture(m_cursorTexture);
	m_cursor.setTextureRect(sf::IntRect{ { 0,0 }, { 64,64 } });
	m_cursor.setOrigin(12.0f, 0.0f);

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
