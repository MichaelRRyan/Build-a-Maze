#ifndef POPUP_H
#define POPUP_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Button.h"

namespace GUI
{
	class Popup
	{
	public:
		Popup(sf::Vector2f t_position, std::string t_message);

		void update(Cursor const& t_cursor);

		void setActive(bool t_active);

		void draw(sf::RenderWindow& t_window);

	private:

		bool m_active;
		sf::Font m_font;
		sf::Texture m_texture;

		sf::RectangleShape m_backgroundShape;

		Button m_okButton;

		std::vector<sf::Text> m_messageText;
	};
}

#endif // !POPUP_H