#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

class GUI
{
	const sf::Vector2f SHOP_BACKGROUND_START_POS{ 800.0f, 0.0f };
	const sf::Vector2f SHOP_DESTROY_TOOL_POS{ 990.0f, 150.0f };
	const sf::Vector2f SHOP_ITEM_START_POS{ 840.0f, 300.0f };
	const sf::Vector2f SHOP_ITEM_SIZE{ 100.0f, 100.0f };
	const sf::Vector2f SHOP_ITEM_SPACING{ 150.0f, 150.0f };
	const int SHOP_ITEMS_PER_ROW{ 3 };

	sf::RectangleShape m_shopBackground;
	sf::RectangleShape m_shopItem;

	sf::Texture m_tilesTexture;
	sf::Sprite m_shopItemImage;
	
	sf::Font m_mainFont;
	sf::Text m_shopText;
	sf::Text m_priceText;
	sf::Text m_moneyText;

public:
	GUI();
	void setupShopScreen();
	void setupFontAndText();
	void drawScreens(sf::RenderWindow &t_window);
	void processEvents(sf::Event t_event, ConstructionMode &t_constructionState, TileType &t_selectedTileType);
	void update(sf::Vector2i t_mousePosition, int & t_money);
};

#endif // !GUI_H

