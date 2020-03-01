#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 07/02/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include "Button.h"
#include "Globals.h"

class Game;

class MenuScreen
{
public:
	MenuScreen(sf::View const & t_windowView);

	void update(Cursor t_cursor, GameState& t_gameState, GameState t_previousGameState, bool& t_exitGame, Game * t_game, std::function<void(Game*)> t_restartFunc);

	void draw(sf::RenderWindow & t_window) const;

	void loadFiles();
	
private:

	sf::Texture m_buttonTexture;
	sf::Font m_buttonFont;
	sf::Font m_titleFont;

	sf::Text m_titleText;

	GUI::Button m_playButton;
	GUI::Button m_restartButton;
	GUI::Button m_exitButton;

	bool m_gameStarted;

	sf::Vector2f m_windowSize;
};

#include "Game.h"

#endif // !MENU_SCREEN_H
