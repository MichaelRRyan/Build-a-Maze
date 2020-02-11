#ifndef SCREENS_H
#define SCREENS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "XBox360Controller.h"
#include "Cursor.h"
#include "Button.h"

class Screens
{
	// Shop constants
	const sf::Vector2f SHOP_BACKGROUND_START_POS{ 800.0f, 0.0f };
	const sf::Vector2f SHOP_DESTROY_TOOL_POS{ 990.0f, 150.0f };
	const sf::Vector2f SHOP_ITEM_START_POS{ 840.0f, 300.0f };
	const sf::Vector2f SHOP_ITEM_SIZE{ 100.0f, 100.0f };
	const sf::Vector2f SHOP_ITEM_SPACING{ 150.0f, 150.0f };
	const int SHOP_ITEMS_PER_ROW{ 3 };

	// Simulation constants
	const sf::Vector2f SIM_ICONS_START_POS{ 1070, 190.0f };
	const sf::Vector2f SIM_ICONS_TEXT_SPACING{ 80.0f, 10.0f };
	const sf::Vector2f SIM_ICONS_SPACING{ 0.0f, 100.0f };
	

	sf::RectangleShape m_shopBackground;
	sf::RectangleShape m_shopItem;

	sf::Texture m_tilesTexture;
	sf::Texture m_iconsTexture;
	sf::Texture m_guiTextures;

	sf::Sprite m_shopItemImage;
	sf::Sprite m_iconsSprite;
	
	// Font
	sf::Font m_mainFont;

	// Build Mode Text
	sf::Text m_shopText;
	sf::Text m_priceText;
	sf::Text m_moneyText;

	// Simulation Text
	sf::Text m_numAIText;
	sf::Text m_timeToCompleteText;
	sf::Text m_moneyEarnedText;

public:
	Screens();
	void setupShopScreen();
	void setupFontAndText();

	void drawConstructionGUI(sf::RenderWindow &t_window);
	void drawSimulationGUI(sf::RenderWindow &t_window, int t_noOfAI, float t_timeToComplete, int t_moneyEarned);

	void processEvents(sf::Event t_event, Cursor t_cursor, ConstructionMode &t_constructionState, TileType &t_selectedTileType);
	void update(sf::Vector2i t_mousePosition, int & t_money);
};

#endif // !SCREENS_H

