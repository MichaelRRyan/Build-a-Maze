#include "HUD.h"

/////////////////////////////////////////////////////////////////
HUD::HUD() : 
	m_shopItems{ {
		{m_tileTectures, WALL_TEXT_RECT, {static_cast<float>(sf::VideoMode::getDesktopMode().width) / 1.4f, static_cast<float>(sf::VideoMode::getDesktopMode().height / 2) }},
		{m_tileTectures, PLANT_TEXT_RECT, {static_cast<float>(sf::VideoMode::getDesktopMode().width) / 1.3f, static_cast<float>(sf::VideoMode::getDesktopMode().height / 2) }},
		{m_tileTectures, {0, 0, 32, 32}, {static_cast<float>(sf::VideoMode::getDesktopMode().width) / 1.2f, static_cast<float>(sf::VideoMode::getDesktopMode().height / 2) }}
	} }
{
	m_shopBackground.setSize({ static_cast<float>(sf::VideoMode::getDesktopMode().width) / 2.7f, static_cast<float>(sf::VideoMode::getDesktopMode().height) });
	m_shopBackground.setFillColor(sf::Color{ 247, 230, 134 });
	m_shopBackground.setPosition(static_cast<float>(sf::VideoMode::getDesktopMode().width) - m_shopBackground.getSize().x, 0.0f);

	m_shopBackground.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopBackground.setOutlineThickness(5.0f);

	if (!m_tileTectures.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		std::cout << "Error loading tile sheet textures";
	}

	for (GUI::Button & button : m_shopItems)
	{
		button.setup();
	}
}

/////////////////////////////////////////////////////////////////
void HUD::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_shopBackground);

	for (GUI::Button const& button : m_shopItems)
	{
		button.draw(t_window);
	}
}

/////////////////////////////////////////////////////////////////