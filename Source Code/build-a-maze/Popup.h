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
		Popup(sf::Vector2f t_position, std::string t_message, std::string t_secondaryMessage);

		void update(Cursor const& t_cursor);

		void setActive(bool t_active);

		const bool isActive() const;

		void draw(sf::RenderWindow& t_window);

	private:

		void setupText(std::vector<sf::Text>& t_text, std::string t_string, sf::Vector2f t_position, sf::Color t_color);

		bool m_active;
		sf::Font m_font;
		sf::Texture m_texture;

		sf::RectangleShape m_backgroundShape;

		Button m_okButton;

		std::vector<sf::Text> m_messageText;
		std::vector<sf::Text> m_secondaryMessageText;
	};
}

#endif // !POPUP_H