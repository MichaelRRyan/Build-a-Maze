#include "Cursor.h"

Cursor::Cursor() :
	m_controllerSensitivity{ 0.25f }
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/Cursors.png"))
	{
		throw("Error loading cursor texture");
	}

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect{ { 0,0 }, { 64,64 } });
	m_sprite.setOrigin(12.0f, 0.0f);

	m_controllerConnected = m_controller.connect();
}

void Cursor::update(sf::RenderWindow const& t_window, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> const& m_maze, GameState t_gameState,
					ConstructionMode t_constructionState, sf::View const& t_guiView, sf::View const& t_mazeView)
{
	// Keep the cursor image up to date
	if (t_constructionState == ConstructionMode::Placing)
	{
		m_sprite.setTextureRect(sf::IntRect{ { 128, 0 }, { 64, 64 } });
	}
	else if (t_constructionState == ConstructionMode::Destroying)
	{
		m_sprite.setTextureRect(sf::IntRect{ { 64, 0 }, { 64, 64 } });
	}
	else if (GameState::Simulation == t_gameState
		&& m_selectedTile.x > 0 && m_selectedTile.x < MAZE_SIZE - 1
		&& m_selectedTile.y >= 0 && m_selectedTile.y < MAZE_SIZE - 1
		&& ((m_maze[m_selectedTile.y][m_selectedTile.x].getType() >= TileType::TreadmillWest
			&& m_maze[m_selectedTile.y][m_selectedTile.x].getType() <= TileType::TreadmillSouth)
			|| m_maze[m_selectedTile.y + 1][m_selectedTile.x].getType() == TileType::TurretWest
			|| m_maze[m_selectedTile.y + 1][m_selectedTile.x].getType() == TileType::TurretEast
			|| m_maze[m_selectedTile.y][m_selectedTile.x].getType() == TileType::Trapdoor))
	{
		m_sprite.setTextureRect(sf::IntRect{ { 64, 0 }, { 64, 64 } });
	}
	else
	{
		m_sprite.setTextureRect(sf::IntRect{ { 0, 0 }, { 64, 64 } });
	}

	// Reset the click bools
	m_clicked = false;
	m_cancelClicked = false;

	// Check if the mouse has been pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!m_clickDown)
		{
			m_clicked = true;
		}

		m_clickDown = true;
	}
	else
	{
		m_clickDown = false;
		m_clicked = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)
		&& !m_cancelClicked)
	{
		if (!m_cancelDown)
		{
			m_cancelClicked = true;
		}

		m_cancelDown = true;
	}
	else
	{
		m_cancelDown = false;
		m_cancelClicked = false;
	}

	// Update the controller if connected
	if (m_controllerConnected)
	{
		updateController();
	}

	// convert the cursor position to world coordinates for the GUI
	m_viewPosition = t_window.mapPixelToCoords(m_position, t_guiView);

	// Update the cursor sprite position
	m_sprite.setPosition(static_cast<sf::Vector2f>(m_viewPosition));

	// convert the cursor position to world coordinates for the maze
	sf::Vector2f worldPos = t_window.mapPixelToCoords(m_position, t_mazeView);

	m_selectedTile = static_cast<sf::Vector2i>(worldPos / TILE_SIZE);
}

void Cursor::updateController()
{
	m_controller.update();

	// Controller Right
	if (m_controller.currentState.LeftThumbStick.x > 15.0f)
	{
		if (m_position.x < WINDOW_WIDTH - m_sprite.getGlobalBounds().width / 2.0f)
		{
			m_position.x += static_cast<int>(m_controller.currentState.LeftThumbStick.x * m_controllerSensitivity);
		}
		else
		{
			m_position.x = WINDOW_WIDTH - static_cast<int>(m_sprite.getGlobalBounds().width / 2.0f);
		}
	}

	// Controller Left
	if (m_controller.currentState.LeftThumbStick.x < -15.0f)
	{
		if (m_position.x > 0.0f)
		{
			m_position.x += static_cast<int>(m_controller.currentState.LeftThumbStick.x * m_controllerSensitivity);
		}
		else
		{
			m_position.x = 0;
		}
	}

	// Controller Down
	if (m_controller.currentState.LeftThumbStick.y > 15.0f)
	{
		if (m_position.y < WINDOW_HEIGHT - m_sprite.getGlobalBounds().height / 2.0f)
		{
			m_position.y += static_cast<int>(m_controller.currentState.LeftThumbStick.y * m_controllerSensitivity);
		}
		else
		{
			m_position.y = WINDOW_HEIGHT - static_cast<int>(m_sprite.getGlobalBounds().height / 2.0f);
		}
	}

	// Controller Up
	if (m_controller.currentState.LeftThumbStick.y < -15.0f)
	{
		if (m_position.y > 0.0f)
		{
			m_position.y += static_cast<int>(m_controller.currentState.LeftThumbStick.y * m_controllerSensitivity);
		}
		else
		{
			m_position.y = 0;
		}
	}

	// Check if either the controller has been clicked
	if (m_controller.currentState.A)
	{
		m_clicked = true;
	}
	if (m_controller.currentState.B)
	{
		m_cancelClicked = true;
	}
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
