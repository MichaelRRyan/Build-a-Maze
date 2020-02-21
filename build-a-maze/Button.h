#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <SFML/Graphics.hpp>
#include "Cursor.h"

namespace GUI
{
	class Button : public sf::Drawable
	{
	public:
		// Text button constructer
		Button(sf::Texture const& t_texture, sf::IntRect t_buttonRect, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position);

		// Image button constructer
		Button(sf::Texture const& t_guiTexture, sf::Texture const& t_imageTexture, sf::IntRect t_guiRect, sf::IntRect t_imageRect, sf::Vector2f t_position);

		//void draw(sf::RenderWindow& t_window) const;
		void setup();
		//bool processMouseEvents(Cursor const & t_cursor);
		bool update(Cursor const& t_cursor);
		

		void setLocked(bool t_state);
		bool getLocked() const;

		void setCharacterSize(unsigned t_characterSize);

		const sf::Vector2f getPosition() const;
		const sf::Vector2f getSize() const;

	private:
		void animate();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Sprite m_sprite;
		sf::Sprite m_image;
		sf::Text m_text;
		bool m_locked; // Whether or not the button is locked from being pressed
		bool m_imageButton; // True if the button is an image button
		float m_rotDirection; // Direction multiplier the button rotates by when hovered over
		bool m_hovered; // Whether the button is hovered or not
	};
}

#endif // !GUI_BUTTON_H