#include "Button.h"

namespace GUI
{
	////////////////////////////////////////////////////////////
	Button::Button(sf::Texture const& t_texture, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position) :
		m_locked{ false }
	{
		m_sprite.setTexture(t_texture);
		m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });
		m_sprite.setPosition(t_position.x + s_WIDTH / 2.0f, t_position.y + s_HEIGHT / 2.0f);
		m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });

		m_text.setPosition(t_position.x + s_WIDTH / 2.0f, t_position.y + s_HEIGHT / 2.2f);
		m_text.setString(t_textString);
		m_text.setFont(t_font);
		m_text.setCharacterSize(35u);

		if (rand() % 2)
			m_rotDirection = 1;
		else
			m_rotDirection = -1;
	}

	////////////////////////////////////////////////////////////
	void Button::draw(sf::RenderWindow& t_window)
	{
		t_window.draw(m_sprite);
		t_window.draw(m_text);
	}

	////////////////////////////////////////////////////////////
	void Button::setup()
	{
		m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);
		m_text.setFillColor(sf::Color::Black);
		m_sprite.setOrigin(s_WIDTH / 2.0f, s_HEIGHT / 2.0f);
	}

	////////////////////////////////////////////////////////////
	bool Button::processMouseEvents(Cursor const& t_cursor)
	{
		if (!m_locked)
		{
			m_sprite.setScale(1.0f, 1.0f);
			m_text.setScale(1.0f, 1.0f);

			m_sprite.setRotation(0.0f);
			m_text.setRotation(0.0f);

			// Check the mouse pointer against the button x bounds
			if (t_cursor.m_position.x > m_sprite.getPosition().x - m_sprite.getGlobalBounds().width / 2.0f
				&& t_cursor.m_position.x < m_sprite.getPosition().x + m_sprite.getGlobalBounds().width / 2.0f)
			{
				// Check the mouse pointer against the button y bounds
				if (t_cursor.m_position.y > m_sprite.getPosition().y - m_sprite.getGlobalBounds().height / 2.0f
					&& t_cursor.m_position.y < m_sprite.getPosition().y + m_sprite.getGlobalBounds().height / 2.0f)
				{
					m_sprite.setScale(0.9f, 0.9f);
					m_text.setScale(0.9f, 0.9f);
					m_sprite.setRotation(5.0f * m_rotDirection);
					m_text.setRotation(5.0f * m_rotDirection);

					if (t_cursor.m_clicked)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	////////////////////////////////////////////////////////////
	void Button::setLocked(bool t_state)
	{
		m_locked = t_state;

		if (t_state)
		{
			m_sprite.setColor(sf::Color{ 100, 100, 100 });
			m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });
		}
		else
		{
			m_sprite.setColor(sf::Color::White);
		}
	}

	bool Button::getLocked() const
	{
		return m_locked;
	}
}