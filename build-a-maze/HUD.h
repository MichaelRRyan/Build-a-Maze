#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <functional>
#include "Button.h"
#include "Globals.h"
#include "Cursor.h"
#include "RoundedRectangleShape.h"
#include "MazeEditor.h"

class Game;

class HUD
{
public:
	HUD(sf::View const& t_windowView, MazeEditor & t_mazeEditor);

	void updateBuildMode(Cursor t_cursor, Game* t_game, std::function<void(Game*)> t_func, int t_money);
	void updateSimText(Cursor t_cursor, Game* t_game, std::function<void(Game*)> t_stopButtonFunc, std::function<void(Game*)> t_pauseButtonFunc, int t_noOfAI, float t_timeToComplete, int t_moneyEarned);

	void drawShop(sf::RenderWindow & t_window) const;
	void drawStats(sf::RenderWindow & t_window);

private:

	void loadFiles();
	void setupShopMenu(sf::View const& t_windowView);
	void setupStatsMenu(sf::View const& t_windowView);

	sf::Texture m_guiTextures;
	sf::Texture m_iconTextures;
	sf::Texture m_tileTextures;

	sf::RectangleShape m_shopBackground;
	sf::RectangleShape m_statsBackground;

	std::array<GUI::Button, 9> m_shopItems;
	std::array<sf::Text, 9> m_shopItemNames;
	std::array<sf::Text, 6> m_shopItemPrices;

	GUI::Button m_playButton;
	GUI::Button m_stopButton;
	GUI::Button m_pauseButton;

	sf::Font m_hudFont;
	sf::Text m_shopTitleText;
	sf::Text m_moneyText;

	sf::Sprite m_iconSprite;
	sf::Vector2f m_iconsPosition;

	sf::Text m_numAIText;
	sf::Text m_timeText;
	sf::Text m_moneyEarnedText;

	sf::RoundedRectangleShape m_menuTab;

	MazeEditor& m_mazeEditorRef;
};

#include "Game.h"

#endif // !HUD_H