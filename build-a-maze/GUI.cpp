#include "GUI.h"



GUI::GUI()
{
	m_shopBackground.setFillColor(sf::Color{ 247, 230, 134 });
	m_shopBackground.setSize({ static_cast<float>(WINDOW_WIDTH - 800u), static_cast<float>(WINDOW_HEIGHT) });
	m_shopBackground.setPosition(800.0f, 0.0f);
	m_shopBackground.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopBackground.setOutlineThickness(5.0f);

	m_shopItem.setFillColor(sf::Color::White);
	m_shopItem.setSize({ 100.0f, 100.0f });
	m_shopItem.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopItem.setOutlineThickness(5.0f);

	if (!m_mainFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_shopText.setFont(m_mainFont);
	m_priceText.setFont(m_mainFont);
	m_moneyText.setFont(m_mainFont);

	m_shopText.setString("CONSTRUCTION SHOP");
	m_shopText.setPosition(1050, 20);
	m_shopText.setFillColor(sf::Color::Black);
	m_shopText.setOrigin(m_shopText.getGlobalBounds().width / 2, 0.0f);

	m_moneyText.setPosition(1050, 80);
	m_moneyText.setString("BALANCE: 400");
	m_moneyText.setFillColor(sf::Color{ 120, 112, 65 });
	m_moneyText.setOrigin(m_moneyText.getGlobalBounds().width / 2, 0.0f);

	if (!m_tilesTexture.loadFromFile("ASSETS/IMAGES/terrain_atlas.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	m_shopItemImage.setTexture(m_tilesTexture);
	m_shopItemImage.setTextureRect(sf::IntRect{ 0, 832, 32, 64 });
	m_shopItemImage.setPosition(860.0f, 130.0f);
	m_shopItemImage.setScale(1.8f, 1.8f);
}

void GUI::drawScreens(sf::RenderWindow & t_window)
{
	t_window.draw(m_shopBackground);

	m_shopItem.setPosition(840.0f, 150.0f);
	for (int i = 1; i < 10; i++)
	{
		if (i == 3) {
			m_shopItem.setFillColor(sf::Color::Red);
		}

		t_window.draw(m_shopItem);
		m_shopItem.move(150.0f, 0.0f);

		if (i % 3 == 0) {
			m_shopItem.move(-450.0f, 150.0f);
		}

		m_shopItem.setFillColor(sf::Color::White);
	}

	t_window.draw(m_shopItemImage);

	t_window.draw(m_shopText);
	t_window.draw(m_moneyText);
}

void GUI::processEvents(sf::Event t_event, int & t_money, ConstructionMode & t_constructionState)
{
	m_moneyText.setString("BALANCE: " + std::to_string(t_money));

	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		if (sf::Mouse::Left == t_event.mouseButton.button)
		{
			if (t_event.mouseButton.x > 840 && t_event.mouseButton.x < 940
				&& t_event.mouseButton.y > 150 && t_event.mouseButton.y < 300)
			{
				t_constructionState = ConstructionMode::Placing;
				std::cout << "Mode Switched to 'Placing'" << std::endl;
			}

			if (t_event.mouseButton.x > 1140 && t_event.mouseButton.x < 1240
				&& t_event.mouseButton.y > 150 && t_event.mouseButton.y < 300)
			{
				t_constructionState = ConstructionMode::Destroying;
				std::cout << "Mode Switched to 'Destroying'" << std::endl;
			}
		}
	}
}
