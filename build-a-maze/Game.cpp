/// @Author Michael Rainsford Ryan

#include "Game.h"



Game::Game() :
	//m_window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Build-a-Maze!" },
	m_window{ sf::VideoMode::getDesktopMode(), "Build-a-Maze", sf::Style::Fullscreen },
	m_exitGame{ false },
	m_gamestate{ GameState::TitleScreen }, // Set the start game state to 'Title screen'
	m_timeModifier{ 1.0f },
	m_GUI_VIEW{ { (static_cast<float>(WINDOW_WIDTH) * 0.75f) / 2.0f, (static_cast<float>(WINDOW_HEIGHT) * 0.75f) / 2.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_mazeView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.85f, static_cast<float>(WINDOW_HEIGHT) * 0.85f} },
	//m_mazeView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} },
	m_menuScreen{ m_GUI_VIEW },
	m_hud{ m_GUI_VIEW },
	m_renderer{ m_window, m_mazeView, m_mazeBlocks, m_mazeSolverPtrs },
	m_popup{ {m_GUI_VIEW.getSize().x / 2.0f - 150.0f, m_GUI_VIEW.getSize().y / 2.0f - 100.0f, }, "The maze is unsolvable.\nEdit it and try again." },
	m_currency{ 1000 } // Set the player's currency to 400
{
	m_window.setVerticalSyncEnabled(true);

	MazeGenerator::generateMaze(m_mazeBlocks);

	setupGame();

	Tile tile;

	tile.setType(TileType::TurretWest);

	tile.setAnimating(true);

	std::cout << tile.getFrame() << std::endl;
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
			switchGameState();
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
				case TileType::TreadmillWest:
					m_selectedTileType = TileType::TreadmillNorth;
					break;
				case TileType::TreadmillEast:
					m_selectedTileType = TileType::TreadmillSouth;
					break;
				case TileType::TreadmillNorth:
					m_selectedTileType = TileType::TreadmillEast;
					break;
				case TileType::TreadmillSouth:
					m_selectedTileType = TileType::TreadmillWest;
					break;
				case TileType::TurretWest:
					m_selectedTileType = TileType::TurretEast;
					break;
				case TileType::TurretEast:
					m_selectedTileType = TileType::TurretWest;
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
					solver->update();
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

			for (Paintball & paintball : m_paintballs)
			{
				paintball.update(m_mazeSolverPtrs);
			}
		}
	}

	// Update the build mode GUI
	if (m_gamestate == GameState::BuildMode)
	{
		m_hud.updateBuildMode(m_cursor, this, &Game::switchGameState, m_constructionState, m_selectedTileType, m_currency);
		m_popup.update(m_cursor);
	}
	else if (m_gamestate == GameState::Simulation)
	{
		m_hud.updateSimText(m_cursor, this, &Game::switchGameState, m_noOfAI, m_timeToComplete, m_moneyEarned);
	}
	else if (m_gamestate == GameState::TitleScreen)
	{
		m_menuScreen.update(m_cursor, m_gamestate, m_exitGame);
	}

	m_cursor.update(m_window, m_mazeBlocks, m_gamestate, m_constructionState, m_GUI_VIEW, m_mazeView);

	handleClickEvents();
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
		m_renderer.drawMaze(m_cursor.m_selectedTile, m_constructionState,m_selectedTileType);

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

		break;
	case GameState::Simulation:
		m_window.setView(m_mazeView);
		m_renderer.drawMazeWithSolvers(m_cursor.m_selectedTile, m_constructionState, m_selectedTileType);

		for (Paintball const& paintball : m_paintballs)
		{
			m_window.draw(paintball);
		}

		m_window.setView(m_GUI_VIEW);
		m_hud.drawStats(m_window);

		if (m_gamePaused)
		{
			m_window.draw(m_pauseScreenFade);
			m_window.draw(m_pauseText);
		}

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
	m_window.setMouseCursorVisible(false);

	m_pauseScreenFade.setSize({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
	m_pauseScreenFade.setFillColor(sf::Color{ 255, 255, 255, 100 });

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
	m_currency = 1000;
	m_simDetailsDisplay = false;
	m_gamePaused = false;

	// Add a random selection of AI to the maze
	m_mazeSolverPtrs.clear(); // Clear any previous AI
	for (int i = 0; i < SOLVERS_MAX ; i++)
	{
		switch (rand() % 3)
		{
		case 0:
			m_mazeSolverPtrs.push_back(new BasicSolver{ m_mazeBlocks });
			break;

		case 1:
			m_mazeSolverPtrs.push_back(new Mathematician{ m_mazeBlocks });
			break;

		case 2:
			m_mazeSolverPtrs.push_back(new Cartographer{ m_mazeBlocks });
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
	m_selectedTileType = TileType::None;
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
	}
}

void Game::switchGameState()
{
	if (m_gamestate == GameState::BuildMode)
	{
		if (m_simDetailsDisplay)
		{
			m_simDetailsDisplay = false;
		}
		else if (MazeValidator::isMazeSolvable(m_mazeBlocks))
		{
			m_gamestate = GameState::Simulation;
			resetSimulation();
		}
		else
		{
			m_popup.setActive(true);
		}
	}
	else if (m_gamestate == GameState::Simulation)
	{
		m_gamestate = GameState::BuildMode;
		m_gamePaused = false;
	}
}

void Game::handleClickEvents()
{
	if (m_cursor.m_clickDown)
	{
		if (m_gamestate == GameState::BuildMode)
		{
			// Make sure the player doesn't remove the outer boundary
			if (m_cursor.m_selectedTile.x > 0 && m_cursor.m_selectedTile.x < MAZE_SIZE - 1
				&& m_cursor.m_selectedTile.y > 0 && m_cursor.m_selectedTile.y < MAZE_SIZE - 1)
			{
				TileType selectedTile = m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].getType();

				// Check if a turret was clicked
				if (selectedTile == TileType::TurretWest
					&& m_constructionState == ConstructionMode::Destroying)
				{
					m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setType(TileType::Wall);
					m_currency += Global::getTilePrice(TileType::TurretWest) / 2;
				}
				// Check if the player clicked a tile
				else if (selectedTile != TileType::None
					&& m_constructionState == ConstructionMode::Destroying)
				{
					m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setType(TileType::None);
					m_currency += Global::getTilePrice(selectedTile) / 2;
				}
				// Else the player tries to buy a tile
				else if (m_constructionState == ConstructionMode::Placing)
				{
					// Check if the player has enough money for the selected item
					if (m_currency >= Global::getTilePrice(m_selectedTileType))
					{
						if (selectedTile == TileType::None
							&& m_selectedTileType != TileType::TurretWest
							&& m_selectedTileType != TileType::TurretEast)
						{
							m_tempTiles.push_back(m_cursor.m_selectedTile);

							m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setType(m_selectedTileType);
							m_currency -= Global::getTilePrice(m_selectedTileType);
						}
						else if (selectedTile == TileType::Wall
							&& (m_selectedTileType == TileType::TurretWest || m_selectedTileType == TileType::TurretEast))
						{
							m_tempTiles.push_back(m_cursor.m_selectedTile);

							m_mazeBlocks[m_cursor.m_selectedTile.y][m_cursor.m_selectedTile.x].setType(m_selectedTileType);
							m_currency -= Global::getTilePrice(m_selectedTileType);
						}
					}
				}
			}
		}
	}
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

									paintball.fire(position, Global::getDirection(selectedTile), sf::Color{ static_cast<sf::Uint8>(rand() % 255), 255, static_cast<sf::Uint8>(rand() % 255) }); // Fire a bullet
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
