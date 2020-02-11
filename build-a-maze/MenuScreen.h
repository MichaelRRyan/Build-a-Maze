#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 07/02/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Button.h"
#include "Globals.h"

class MenuScreen
{
public:
	MenuScreen();

	void processEvents(Cursor t_cursor, GameState& t_gameState, bool& t_exitGame);

	void draw(sf::RenderWindow & t_window) const;

	void loadFiles();
	
private:

	sf::Texture m_buttonTexture;
	sf::Font m_buttonFont;
	sf::Font m_titleFont;

	sf::Text m_titleText;

	GUI::Button m_playButton;
	GUI::Button m_exitButton;
};

#endif // !MENU_SCREEN_H
