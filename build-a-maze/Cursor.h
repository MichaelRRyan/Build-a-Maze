#ifndef CURSOR_H
#define CURSOR_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 14/10/2019
/// </summary>

#include <SFML/Graphics.hpp>

struct Cursor
{
	bool m_clicked;
	bool m_cancelClicked;
	sf::Vector2i m_position;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};

#endif // !CURSOR_H
