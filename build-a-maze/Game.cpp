/// @Author Michael Rainsford Ryan

#include "Game.h"

///////////////////////////////////////////////////////////////////////////
Game::Game() :
	//m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Build-a-Maze!" },
	m_window{ sf::VideoMode::getDesktopMode(), "Build-a-Maze", sf::Style::Fullscreen },
	m_BUILD_MODE_OFFSET{ 420.0f },
	m_SIM_MODE_OFFSET{ 320.0f },
	m_exitGame{ false },
	m_gamestate{ GameState::TitleScreen }, // Set the start game state to 'Title screen'
	m_timeModifier{ 1.0f },
	m_GUI_VIEW{ { (static_cast<float>(WINDOW_WIDTH) * 0.75f) / 2.0f, (static_cast<float>(WINDOW_HEIGHT) * 0.75f) / 2.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_mazeView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.85f, static_cast<float>(WINDOW_HEIGHT) * 0.85f} },
	//m_mazeView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_menuScreen{ m_GUI_VIEW },
	m_hud{ m_GUI_VIEW, m_mazeEditor },
	m_renderer{ m_window, m_mazeView, m_mazeBlocks, m_mazeSolverPtrs, m_sheep },
	m_popup{ {m_GUI_VIEW.getSize().x / 2.0f - 150.0f, m_GUI_VIEW.getSize().y / 2.0f - 100.0f, }, "The maze is unsolvable.\nEdit it and try again." },
	m_currency{ 1000 }, // Set the player's currency to 400
	m_mazeEditor{ m_mazeBlocks, m_currency },
	m_animatingHUD{ false },
	m_endGameUI{ m_GUI_VIEW }
{
	m_window.setVerticalSyncEnabled(true);

	setupObjects();
	setupGame();
}

///////////////////////////////////////////////////////////////////////////
Game::~Game()
{
	for (MazeSolver* solver : m_mazeSolverPtrs)
	{
		delete solver;
	}

	m_mazeSolverPtrs.clear();
}

///////////////////////////////////////////////////////////////////////////
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
		else if (sf::Event::KeyPressed == nextEvent.type)
		{
			processKeyboardEvents(nextEvent);
		}
		else if (sf::Event::MouseMoved == nextEvent.type)
		{
			m_cursor.m_position = { static_cast<int>(nextEvent.mouseMove.x), static_cast<int>(nextEvent.mouseMove.y) };
		}
	} // End while poll event
}

/// <summary>
/// Processes user keyboard events in the game
/// </summary>
/// <param name="t_event">Current Event</param>
void Game::processKeyboardEvents(sf::Event t_event)
{
	switch (m_gamestate)
	{
	case GameState::BuildMode:
		m_mazeEditor.handleKeyPresses(t_event);

		if (sf::Keyboard::Escape == t_event.key.code)
		{
			if (m_mazeEditor.getConstructionMode() == ConstructionMode::None)
			{
				m_gamestate = GameState::TitleScreen;
			}
		}

		// Switch game state
		if (sf::Keyboard::Enter == t_event.key.code)
		{
			switchGameState();
		}

#ifdef PLACE_SHEEP_DEBUG
		if (sf::Keyboard::S == t_event.key.code)
		{
			placeSheep();
		}
#endif //PLACE_SHEEP_DEBUG
		break;
	case GameState::Simulation:
		// Pause the game
		if (sf::Keyboard::P == t_event.key.code)
		{
			togglePause();
		}

		if (sf::Keyboard::Escape == t_event.key.code)
		{
			switchGameState();
		}

		processTimeModifierEvents(t_event);
		break;
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

	switch (m_gamestate)
	{
	case GameState::TitleScreen:
		m_menuScreen.update(m_cursor, m_gamestate, m_exitGame, this, &Game::restartGame);
		break;
	case GameState::SettingsScreen:
		break;
	case GameState::BuildMode:
		m_mazeEditor.update(m_cursor);
		m_hud.updateBuildMode(m_cursor, this, &Game::switchGameState, &Game::purchaseSheep, m_currency);
		m_popup.update(m_cursor);
		updateSheep();

		break;
	case GameState::Simulation:
		updateSimulation(t_deltaTime);
		m_hud.updateSimText(m_cursor, this, &Game::switchGameState, &Game::togglePause, m_noOfAI, m_timeToComplete, m_moneyEarned);
		updateSheep();

		break;
	case GameState::GameEnd:
		m_endGameUI.update(m_cursor, this, &Game::restartGame, &Game::exitToMenu);
		break;
	}

	m_cursor.update(m_window, m_mazeBlocks, m_gamestate, m_mazeEditor.getConstructionMode(), m_GUI_VIEW, m_mazeView);
}

/// <summary>
/// Renders all the objects in the game.
/// <para>Renders the maze walls and AI in order of row to add overlap of walls and AI.</para>
/// <para>Draws the GUI to the screen.</para>
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	switch (m_gamestate)
	{
	case GameState::TitleScreen:
		m_window.setView(m_GUI_VIEW);
		m_menuScreen.draw(m_window);
		break;
	case GameState::SettingsScreen:
		break;
	case GameState::BuildMode:
		m_window.setView(m_mazeView);
		m_renderer.drawMaze(m_cursor.m_selectedTile, m_mazeEditor.getConstructionMode(), m_mazeEditor.getSelectedTileType());

		m_window.setView(m_GUI_VIEW);
		if (!m_simDetailsDisplay)
		{
			m_hud.drawShop(m_window);
		}
		else
		{
			m_hud.drawStats(m_window);
		}

		m_popup.draw(m_window);

		if (m_animatingHUD)
		{
			animateMaze();
		}

		break;
	case GameState::Simulation:
		drawSimulation();
		break;
	case GameState::GameEnd:
		m_window.setView(m_GUI_VIEW);

		if (m_endGameUI.isAnimating())
		{
			drawSimulation();
		}

		m_endGameUI.draw(m_window);
		break;
	}

	// Draw the cursor
	m_window.draw(m_cursor);

	m_window.display();
}

/// <summary>
/// Sets up the game
/// </summary>
void Game::setupGame()
{
	m_mazeView.setCenter(m_BUILD_MODE_OFFSET, m_mazeView.getCenter().y);
	MazeGenerator::generateMaze(m_mazeBlocks);
	m_mazeEditor.reset();
	m_currency = 1000;
	m_simDetailsDisplay = false;
	m_gamePaused = false;

	// Delete and clear any previous AI
	for (MazeSolver* solver : m_mazeSolverPtrs)
	{
		delete solver;
	}
	m_mazeSolverPtrs.clear();

	// Add a random selection of AI to the maze
	for (int i = 0; i < SOLVERS_MAX ; i++)
	{
		switch (rand() % 3)
		{
		case 0:
			m_mazeSolverPtrs.push_back(new BasicSolver{ m_mazeBlocks, m_sheep });
			break;

		case 1:
			m_mazeSolverPtrs.push_back(new Mathematician{ m_mazeBlocks, m_sheep });
			break;

		case 2:
			m_mazeSolverPtrs.push_back(new Cartographer{ m_mazeBlocks, m_sheep });
			break;
		}
	}

	// Delete all sheep and clear the vector
	for (Sheep* sheep : m_sheep)
	{
		delete sheep;
	}
	m_sheep.clear();

	// Setup 3 sheep
	for (int i = 0; i < 3; i++)
	{
		m_sheep.push_back(new Sheep{ m_mazeBlocks }); // Push back a new sheep into the vector
	}

	placeSheep();
}

///////////////////////////////////////////////////////////////////////////
void Game::setupObjects()
{
	m_window.setMouseCursorVisible(false);

	m_pauseScreenFade.setSize({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
	m_pauseScreenFade.setFillColor(sf::Color{ 255, 255, 255, 100 });

	if (!m_mainFont.loadFromFile("ASSETS/FONTS/tf2build.ttf"))
	{
		std::cout << "Error loading main font (tf2 font)";
	}

	m_pauseText.setString("PAUSED");
	m_pauseText.setCharacterSize(100u);
	m_pauseText.setFont(m_mainFont);
	m_pauseText.setFillColor(sf::Color{ 0, 0, 0, 150 });
	m_pauseText.setPosition(m_GUI_VIEW.getSize().x / 3.0f, m_GUI_VIEW.getSize().y / 2.5f);
	m_pauseText.setOrigin(m_pauseText.getGlobalBounds().width / 2, m_pauseText.getGlobalBounds().height / 2);
}

/// <summary>
/// @brief resets all the game data used for simulation
/// </summary>
void Game::resetSimulation()
{
	m_mazeEditor.reset();
	m_prevTimeToComplete = 0;
	m_timeToComplete = 0;
	m_moneyEarned = 0;
	m_timeModifier = 1.0f;

	// Reset all AI's positions to the start of the maze
	int moveDelayCounter = 0; // Counts the number of maze solvers and uses this to set the delay on movement
	for (MazeSolver* solver : m_mazeSolverPtrs)
	{
		solver->reset(moveDelayCounter * 64);
		solver->setMoveTimer(moveDelayCounter * 64);
		solver->setPreviousPos(1, -moveDelayCounter * 4);
		moveDelayCounter++;
	}
}

/// <summary>
/// @brief Update the game when in simulation mode
/// </summary>
/// <param name="t_deltaTime">Time since last update</param>
void Game::updateSimulation(sf::Time t_deltaTime)
{
	if (!m_gamePaused)
	{
		// Reset AI count
		int lastMax = m_noOfAI;
		int i = SOLVERS_MAX;
		m_noOfAI = 0;

		// Loop through and update all AI. Count those that are still active
		for (MazeSolver* solver : m_mazeSolverPtrs)
		{
			if (solver->getActive())
			{
				solver->update();
				m_noOfAI++;
			}
			else if (solver->getPos().x != MAZE_SIZE - 1 && solver->getPos().y != MAZE_SIZE - 2)
			{
				i--;
			}
		}

		if (i < lastMax)
		{
			std::cout << lastMax - i << " AI Killed " << m_noOfAI << std::endl;
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

		for (Paintball& paintball : m_paintballs)
		{
			paintball.update(m_mazeSolverPtrs);
		}

		handleClickEvents();
	}
}

/// <summary>
/// @brief process events related to the time modifier
/// </summary>
/// <param name="t_event">user event</param>
void Game::processTimeModifierEvents(sf::Event t_event)
{
	if (t_event.key.code >= sf::Keyboard::Num1 && t_event.key.code <= sf::Keyboard::Num3)
	{
		if (sf::Keyboard::Num1 == t_event.key.code)
		{
			if (m_timeModifier > 0.25)
			{
				m_timeModifier *= 0.5f;
			}
		}
		else if (sf::Keyboard::Num2 == t_event.key.code)
		{
			m_timeModifier = 1;
		}
		else if (sf::Keyboard::Num3 == t_event.key.code)
		{
			if (m_timeModifier < 4)
			{
				m_timeModifier *= 2.0f;
			}
		}

		for (MazeSolver* solver : m_mazeSolverPtrs)
		{
			solver->setTimeModifier(m_timeModifier);
		}

		for (Sheep* sheep : m_sheep)
		{
			sheep->setTimeModifier(m_timeModifier);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::switchGameState()
{
	if (m_gamestate == GameState::BuildMode)
	{
		if (m_simDetailsDisplay)
		{
			m_simDetailsDisplay = false;
		}
		else if (m_mazeValidator.isMazeSolvable(m_mazeBlocks))
		{
			m_gamestate = GameState::Simulation;
			m_hud.animateInStats();
			resetSimulation();
			resetSheep();
			startAnimatingMaze();
		}
		else
		{
			m_popup.setActive(true);
		}
	}
	else if (m_gamestate == GameState::Simulation)
	{
		m_gamestate = GameState::BuildMode;
		m_hud.animateInShop();
		m_gamePaused = false;
		resetSheep();
		startAnimatingMaze();
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::handleClickEvents()
{
	if (m_cursor.m_clicked)
	{
		if (m_gamestate == GameState::Simulation)
		{
			// Make sure the player doesn't remove the outer boundary
			if (m_cursor.m_selectedTile.x > 0 && m_cursor.m_selectedTile.x < MAZE_SIZE - 1
				&& m_cursor.m_selectedTile.y > 0 && m_cursor.m_selectedTile.y < MAZE_SIZE - 1)
			{
				TileType selectedTile = m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].getType();

				if (selectedTile != TileType::Wall
					&& selectedTile != TileType::Mud
					&& selectedTile != TileType::None)
				{
					if (selectedTile == TileType::TurretWest
						|| selectedTile == TileType::TurretEast)
					{
						if (!m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].getAnimating())
						{
							// Turn on animation
							m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setAnimating(true);

							// Loop all the paintballs
							for (Paintball& paintball : m_paintballs)
							{
								// Find an inactive one
								if (!paintball.isActive())
								{
									sf::Vector2f position{ static_cast<sf::Vector2f>(m_cursor.m_selectedTile)* TILE_SIZE }; // Find the turret position
									position.y -= 18.0f; // Adjust height for barrel height

									if (selectedTile == TileType::TurretEast)
									{
										position.x += 32.0f;
									}

									paintball.fire(position, Global::getDirection(selectedTile), sf::Color{ 135, 113, 85 }); // Fire a bullet
									break; // Break once a bullet has been found
								}
							}
						}
					}
					else if (selectedTile == TileType::SteppingStones)
					{
						if (!m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].getAnimating())
						{
							// Turn on animation
							m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setAnimating(true);
						}
					}
					else
					{
						// Toggle animation
						m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setAnimating(!m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].getAnimating());
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::togglePause()
{
	m_gamePaused = !m_gamePaused;
}

///////////////////////////////////////////////////////////////////////////
void Game::endGame()
{
	m_gamestate = GameState::GameEnd;
	m_animationClock.restart();
	m_endGameUI.setAnimating();
}

///////////////////////////////////////////////////////////////////////////
void Game::startAnimatingMaze()
{
	m_animationClock.restart();
	m_animatingHUD = true;
}

///////////////////////////////////////////////////////////////////////////
void Game::animateMaze()
{
	if (GameState::BuildMode == m_gamestate)
	{
		float offset = m_BUILD_MODE_OFFSET + ((m_SIM_MODE_OFFSET - m_BUILD_MODE_OFFSET) * (1.0f - m_animationClock.getElapsedTime().asSeconds() / m_hud.getSecondsToAnimate()));
		m_mazeView.setCenter(offset, m_mazeView.getCenter().y);
	}
	else if (GameState::Simulation == m_gamestate)
	{
		float offset = m_BUILD_MODE_OFFSET + ((m_SIM_MODE_OFFSET - m_BUILD_MODE_OFFSET) * (m_animationClock.getElapsedTime().asSeconds() / m_hud.getSecondsToAnimate()));
		m_mazeView.setCenter(offset, m_mazeView.getCenter().y);
	}

	if (m_animationClock.getElapsedTime().asSeconds() >= m_hud.getSecondsToAnimate())
	{
		m_animatingHUD = false;

		if (GameState::BuildMode == m_gamestate)
		{
			m_mazeView.setCenter(m_BUILD_MODE_OFFSET, m_mazeView.getCenter().y);
		}
		else if (GameState::Simulation == m_gamestate)
		{
			m_mazeView.setCenter(m_SIM_MODE_OFFSET, m_mazeView.getCenter().y);
		}
	}
}

/// <summary>
/// @brief place the sheep in the maze in an accessible spot
/// </summary>
void Game::placeSheep()
{
	// Put the maze through the validator to get a stack of all accessible tiles
	m_mazeValidator.isMazeSolvable(m_mazeBlocks);
	std::vector<sf::Vector2i> availableTiles{ m_mazeValidator.getAccessibleTiles() }; // Get all the accessible tiles in the maze

	// Setup sheep
	for (Sheep * sheep : m_sheep)
	{
		sf::Vector2i sheepPosition{ availableTiles.at(rand() % availableTiles.size()) }; // Get a random position for the sheep

		sheep->setPos(sheepPosition.y, sheepPosition.x); // Set the position of the sheep
	}

#ifdef PLACE_SHEEP_DEBUG
	sf::RectangleShape shape{ { TILE_SIZE, TILE_SIZE } };

	shape.setFillColor(sf::Color{ 255, 255, 255, 100 });

	m_window.setView(m_mazeView);

	for (sf::Vector2i vec : availableTiles)
	{
		shape.setPosition(static_cast<sf::Vector2f>(vec) * TILE_SIZE);
		m_window.draw(shape);
	}

	m_window.display();

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{

	}
#endif // PLACE_SHEEP_DEBUG
}

///////////////////////////////////////////////////////////////////////////
void Game::placeSheep(Sheep* t_sheep)
{
	// Put the maze through the validator to get a stack of all accessible tiles
	m_mazeValidator.isMazeSolvable(m_mazeBlocks);
	std::vector<sf::Vector2i> availableTiles{ m_mazeValidator.getAccessibleTiles() }; // Get all the accessible tiles in the maze

	sf::Vector2i sheepPosition{ availableTiles.at(rand() % availableTiles.size()) }; // Get a random position for the sheep

	t_sheep->setPos(sheepPosition.y, sheepPosition.x); // Set the position of the sheep
}

///////////////////////////////////////////////////////////////////////////
void Game::resetSheep()
{
	// Loop all sheep
	for (Sheep* sheep : m_sheep)
	{
		sheep->reset(); // Reset the current sheep
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::updateSheep()
{
	// Loop all sheep
	for (unsigned i = 0; i < m_sheep.size(); i++)
	{
		// Check if the current sheep is active
		if (m_sheep.at(i)->getActive())
		{
			m_sheep.at(i)->update(); // Update the sheep if active
		}
		else // If not active
		{
			delete m_sheep.at(i); // Delete the sheep from memory
			m_sheep.erase(m_sheep.begin() + i); // Erase the sheep from the vector
			i--; // Decrement the index to not disturb the update loop
		}
	}

	// If no sheep remain, game over
	if (m_sheep.size() == 0)
	{
		endGame();
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::purchaseSheep()
{
	if (m_currency >= SHEEP_PRICE)
	{
		m_currency -= SHEEP_PRICE;

		m_sheep.push_back(new Sheep(m_mazeBlocks));

		placeSheep(m_sheep.at(m_sheep.size() - 1));
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::drawSimulation()
{
	m_window.setView(m_mazeView);
	m_renderer.drawMazeWithSolvers(m_cursor.m_selectedTile, m_mazeEditor.getConstructionMode(), m_mazeEditor.getSelectedTileType());

	for (Paintball const& paintball : m_paintballs)
	{
		m_window.draw(paintball);
	}

	m_window.setView(m_GUI_VIEW);

	if (m_gamePaused)
	{
		m_window.draw(m_pauseScreenFade);
		m_window.draw(m_pauseText);
	}

	m_hud.drawStats(m_window);

	if (m_animatingHUD)
	{
		animateMaze();
	}
}

///////////////////////////////////////////////////////////////////////////
void Game::exitToMenu()
{
	setupGame();
	m_gamestate = GameState::TitleScreen;
}

///////////////////////////////////////////////////////////////////////////
void Game::restartGame()
{
	setupGame();
	m_gamestate = GameState::BuildMode;
}

///////////////////////////////////////////////////////////////////////////