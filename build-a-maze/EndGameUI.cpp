#include "EndGameUI.h"

EndGameUI::EndGameUI(sf::View const& t_guiView) :
	m_SECONDS_TO_ANIMATE{ 3.0f },
	m_animating{ false },
	m_background{ t_guiView.getSize() },
	m_restartButton{ m_buttonTexture, { 0, 0, 300, 80 }, m_font, "RESTART", { (t_guiView.getSize().x / 2.0f) + 50.0f, t_guiView.getSize().y / 1.8f } },
	m_menuButton{ m_buttonTexture, { 0, 0, 300, 80 }, m_font, "MAIN MENU", { (t_guiView.getSize().x / 2.0f) - 350.0f, t_guiView.getSize().y / 1.8f } }
{
	if (!m_font.loadFromFile("ASSETS/FONTS/tf2build.ttf"))
	{
		throw("Error loading main font (tf2 font) for end game message");
	}

	m_message.setString("ALL YOUR SHEEP WERE STOLEN... YOU LOSE");
	m_message.setCharacterSize(30u);
	m_message.setFont(m_font);
	m_message.setFillColor(sf::Color{ 255, 255, 255 });
	m_message.setPosition(t_guiView.getSize().x / 2.0f, t_guiView.getSize().y / 2.5f);
	m_message.setOrigin(m_message.getGlobalBounds().width / 2, m_message.getGlobalBounds().height / 2);

	// Load the GUI textures
	if (!m_buttonTexture.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	m_restartButton.setup();
	m_menuButton.setup();
}

void EndGameUI::update(Cursor const& t_cursor, Game* t_game, std::function<void(Game*)> t_restartFunc, std::function<void(Game*)> t_menuFunc)
{
	// Switch off the animation and set final values if animation time is up
	if (m_animationClock.getElapsedTime().asSeconds() >= m_SECONDS_TO_ANIMATE)
	{
		m_animating = false;
		m_background.setFillColor({ 0, 0, 0, 255 });
		m_message.setFillColor({ 255, 255, 255 });
		m_restartButton.setOpacity(255u);
		m_menuButton.setOpacity(255u);
	}
	else // Animate the background, text and buttons
	{
		float animProgress{ m_animationClock.getElapsedTime().asSeconds() / m_SECONDS_TO_ANIMATE };

		m_background.setFillColor({ 0, 0, 0, static_cast<sf::Uint8>(255.0f * animProgress) });
		m_message.setFillColor({ 255, 255, 255, static_cast<sf::Uint8>(255.0f * animProgress) });
		m_restartButton.setOpacity(static_cast<sf::Uint8>(255.0f * animProgress));
		m_menuButton.setOpacity(static_cast<sf::Uint8>(255.0f * animProgress));
	}
	
	// Update the buttons
	if (m_restartButton.update(t_cursor))
	{
		t_restartFunc(t_game);
	}

	if (m_menuButton.update(t_cursor))
	{
		t_menuFunc(t_game);
	}
}

void EndGameUI::draw(sf::RenderWindow & t_window) const
{
	t_window.draw(m_background);
	t_window.draw(m_message);

	t_window.draw(m_restartButton);
	t_window.draw(m_menuButton);
}

const bool EndGameUI::isAnimating() const
{
	return m_animating;
}

void EndGameUI::setAnimating()
{
	m_animating = true;
	m_animationClock.restart();
	m_background.setFillColor({ 0, 0, 0, 0 });
	m_message.setFillColor({ 255, 255, 255, 0 });
	m_restartButton.setOpacity(0u);
	m_menuButton.setOpacity(0u);
}
