#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

class GUI
{
	sf::RectangleShape m_shopBackground;
	sf::RectangleShape m_shopItem;
	
	sf::Font m_mainFont;
	sf::Text m_shopText;
	sf::Text m_priceText;
	sf::Text m_moneyText;

	sf::Texture m_tilesTexture;
	sf::Sprite m_shopItemImage;

public:
	GUI();
	void drawScreens(sf::RenderWindow &t_window);
	void processEvents(sf::Event t_event, int &t_money, ConstructionMode &t_constructionState);
};

#endif // !GUI_H

