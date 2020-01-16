#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "Globals.h"

class Renderer
{
public:
	static void drawMazeBackground(sf::RenderWindow& t_window, sf::Sprite t_tileSprite);
};

#endif // !RENDERER_H