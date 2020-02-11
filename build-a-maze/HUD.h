#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "Button.h"
#include "Globals.h"

class HUD
{
public:
	HUD();

	void draw(sf::RenderWindow & t_window) const;

private:
	sf::Texture m_tileTectures;

	sf::RectangleShape m_shopBackground;

	std::array<GUI::Button, 3> m_shopItems;
};

#endif // !HUD_H