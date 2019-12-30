#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <SFML/Graphics.hpp>
#include "Cursor.h"

namespace GUI
{
	class Button
	{
	public:
		Button(sf::Texture const& t_texture, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position);
		void draw(sf::RenderWindow& t_window);
		void setup();
		bool processMouseEvents(Cursor const & t_cursor);

		void setLocked(bool t_state);
		bool getLocked() const;

		static constexpr float s_WIDTH{ 300.0f };
		static constexpr float s_HEIGHT{ 80.0f };

	private:
		sf::Sprite m_sprite;
		sf::Text m_text;
		bool m_locked; // Whether or not the button is locked from being pressed
		float m_rotDirection; // Direction multiplier the button rotates by when hovered over
	};
}

#endif // !GUI_BUTTON_H