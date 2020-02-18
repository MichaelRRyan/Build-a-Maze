#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "Button.h"
#include "Globals.h"
#include "Cursor.h"

class HUD
{
public:
	HUD(sf::View const& t_windowView);

	void processShopEvents(Cursor t_cursor, ConstructionMode& t_constructionState, TileType& t_selectedTileType);

	void updateMoneyText(int t_money);
	void updateSimText(int t_noOfAI, float t_timeToComplete, int t_moneyEarned);

	void drawShop(sf::RenderWindow & t_window) const;
	void drawStats(sf::RenderWindow& t_window);

private:
	sf::Texture m_tileTectures;
	sf::Texture m_iconTextures;

	sf::RectangleShape m_shopBackground;
	sf::RectangleShape m_statsBackground;

	std::array<GUI::Button, 5> m_shopItems;

	sf::Font m_hudFont;
	sf::Text m_shopTitleText;
	sf::Text m_moneyText;

	sf::Sprite m_iconSprite;
	sf::Vector2f m_iconsPosition;

	sf::Text m_numAIText;
	sf::Text m_timeText;
	sf::Text m_moneyEarnedText;
	
};

#endif // !HUD_H