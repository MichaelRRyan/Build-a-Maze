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

void Cursor::update(sf::RenderWindow const & t_window, ConstructionMode t_constructionState, sf::View const& t_guiView, sf::View const& t_mazeView)
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
		m_clicked = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_cancelClicked = true;
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