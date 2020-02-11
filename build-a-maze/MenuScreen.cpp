#include "MenuScreen.h"

/////////////////////////////////////////////////////////////////
MenuScreen::MenuScreen() :
	m_playButton(m_buttonTexture, m_buttonFont, "PLAY", { static_cast<float>(sf::VideoMode::getDesktopMode().width / 2) - m_playButton.s_WIDTH / 2.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height) / 1.8f }),
	m_exitButton(m_buttonTexture, m_buttonFont, "EXIT", { static_cast<float>(sf::VideoMode::getDesktopMode().width / 2) - m_exitButton.s_WIDTH / 2.0f, static_cast<float>(sf::VideoMode::getDesktopMode().height) / 1.5f })
{
	loadFiles();

	m_playButton.setup();
	m_exitButton.setup();

	// Setup title text
	m_titleText.setFont(m_titleFont);
	m_titleText.setString("Build-a-Maze!");
	m_titleText.setPosition(static_cast<float>(sf::VideoMode::getDesktopMode().width / 2), static_cast<float>(sf::VideoMode::getDesktopMode().height / 4));
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setCharacterSize(120u);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2, 0.0f);
}

/////////////////////////////////////////////////////////////////
void MenuScreen::processEvents(Cursor t_cursor, GameState& t_gameState, bool& t_exitGame)
{
	if (m_playButton.processMouseEvents(t_cursor))
	{
		t_gameState = GameState::BuildMode;
	}

	if (m_exitButton.processMouseEvents(t_cursor))
	{
		t_exitGame = true;
	}
}

/////////////////////////////////////////////////////////////////
void MenuScreen::draw(sf::RenderWindow& t_window) const
{
	t_window.clear(sf::Color{ sf::Color{ 247, 230, 134 } });

	m_playButton.draw(t_window);
	m_exitButton.draw(t_window);

	t_window.draw(m_titleText);
}

/////////////////////////////////////////////////////////////////
void MenuScreen::loadFiles()
{
	// Load the button texture

	if (!m_buttonTexture.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	// Load the button font
	if (!m_buttonFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_titleFont = m_buttonFont;
}

/////////////////////////////////////////////////////////////////