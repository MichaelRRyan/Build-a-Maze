#include "MenuScreen.h"

/////////////////////////////////////////////////////////////////
MenuScreen::MenuScreen(sf::View const& t_windowView) :
	m_playButton(m_buttonTexture, { 0, 0, 300, 80 }, m_buttonFont, "PLAY", { (t_windowView.getSize().x / 2.0f) - 150.0f, t_windowView.getSize().y / 1.8f }),
	m_exitButton(m_buttonTexture, { 0, 0, 300, 80 }, m_buttonFont, "EXIT", { (t_windowView.getSize().x / 2.0f) - 150.0f, t_windowView.getSize().y / 1.4f })
{
	loadFiles();

	m_playButton.setup();
	m_exitButton.setup();

	// Setup title text
	m_titleText.setFont(m_titleFont);
	m_titleText.setString("Build-a-Maze!");
	m_titleText.setPosition(t_windowView.getSize().x / 2.0f, t_windowView.getSize().y / 6.0f);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setCharacterSize(100u);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2, 0.0f);
}

void MenuScreen::update(Cursor t_cursor, GameState& t_gameState, bool& t_exitGame)
{
	if (m_playButton.update(t_cursor))
	{
		t_gameState = GameState::BuildMode;
	}

	if (m_exitButton.update(t_cursor))
	{
		t_exitGame = true;
	}
}

/////////////////////////////////////////////////////////////////
void MenuScreen::draw(sf::RenderWindow& t_window) const
{
	t_window.clear(sf::Color{ 247, 230, 134 });

	t_window.draw(m_playButton);
	t_window.draw(m_exitButton);

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
	if (!m_buttonFont.loadFromFile("ASSETS/FONTS/tf2Build.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_titleFont = m_buttonFont;
}

/////////////////////////////////////////////////////////////////