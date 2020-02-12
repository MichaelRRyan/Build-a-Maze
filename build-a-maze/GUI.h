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
	// Simulation constants
	const sf::Vector2f SIM_ICONS_START_POS{ 1070, 190.0f };
	const sf::Vector2f SIM_ICONS_TEXT_SPACING{ 80.0f, 10.0f };
	const sf::Vector2f SIM_ICONS_SPACING{ 0.0f, 100.0f };

	sf::Texture m_iconsTexture;
	sf::Texture m_guiTextures;

	sf::Sprite m_iconsSprite;
	
	// Font
	sf::Font m_mainFont;

	// Simulation Text
	sf::Text m_numAIText;
	sf::Text m_timeToCompleteText;
	sf::Text m_moneyEarnedText;

public:
	Screens();
	void setupFontAndText();

	void drawSimulationGUI(sf::RenderWindow &t_window, int t_noOfAI, float t_timeToComplete, int t_moneyEarned);
};

#endif // !SCREENS_H

