#include "Button.h"

namespace GUI
{
	////////////////////////////////////////////////////////////
	Button::Button(sf::Texture const& t_texture, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position) :
		m_locked{ false },
		m_imageButton{ false },
		m_hovered{ false }
	{
		m_sprite.setTexture(t_texture);
		m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });
		m_sprite.setPosition(t_position.x + s_WIDTH / 2.0f, t_position.y + s_HEIGHT / 2.0f);

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
	Button::Button(sf::Texture const& t_guiTexture, sf::Texture const& t_imageTexture, sf::IntRect t_guiRect, sf::IntRect t_imageRect, sf::Vector2f t_position) :
		m_locked{ false },
		m_imageButton{ true },
		m_hovered{ false }
	{
		// Button background
		m_sprite.setTexture(t_guiTexture);
		m_sprite.setTextureRect(t_guiRect);
		m_sprite.setPosition(t_position.x + m_sprite.getGlobalBounds().width / 2.0f, t_position.y + m_sprite.getGlobalBounds().height / 2.0f);

		// Image overlay
		m_image.setTexture(t_imageTexture);
		m_image.setTextureRect(t_imageRect);
		m_image.setPosition(t_position.x + m_sprite.getGlobalBounds().width / 2.0, t_position.y + m_sprite.getGlobalBounds().height / 2.0f);
	}

	////////////////////////////////////////////////////////////
	void Button::draw(sf::RenderWindow& t_window) const
	{
		if (!m_imageButton)
		{
			t_window.draw(m_sprite);
			t_window.draw(m_text);
		}
		else
		{
			t_window.draw(m_sprite);
			t_window.draw(m_image);
		}
	}

	////////////////////////////////////////////////////////////
	void Button::setup()
	{
		m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);
		m_text.setFillColor(sf::Color::Black);
		m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f, m_sprite.getGlobalBounds().height / 2.0f);

		m_image.setOrigin(m_image.getGlobalBounds().width / 2.0f, m_image.getGlobalBounds().height / 2.0f);
		m_image.setScale(2.0f, 2.0f);
	}

	////////////////////////////////////////////////////////////
	bool Button::processMouseEvents(Cursor const& t_cursor)
	{
		if (!m_locked)
		{
			m_hovered = false;

			float scale = m_sprite.getScale().x;
			m_sprite.setScale(1.0f, 1.0f);

			float angle = m_sprite.getRotation();
			m_sprite.setRotation(0.0f);

			// Check the mouse pointer against the button x bounds
			if (t_cursor.m_viewPosition.x > m_sprite.getPosition().x - m_sprite.getGlobalBounds().width / 2.0f
				&& t_cursor.m_viewPosition.x < m_sprite.getPosition().x + m_sprite.getGlobalBounds().width / 2.0f)
			{
				// Check the mouse pointer against the button y bounds
				if (t_cursor.m_viewPosition.y > m_sprite.getPosition().y - m_sprite.getGlobalBounds().height / 2.0f
					&& t_cursor.m_viewPosition.y < m_sprite.getPosition().y + m_sprite.getGlobalBounds().height / 2.0f)
				{
					m_hovered = true;
				}
			}

			m_sprite.setScale(scale, scale);
			m_sprite.setRotation(angle);

			if (m_hovered && t_cursor.m_clicked)
			{
				return true;
			}
		}
		return false;
	}

	////////////////////////////////////////////////////////////
	void Button::update()
	{
		// Animate to hovered
		if (m_hovered && m_sprite.getScale().x > 0.9)
		{
			m_sprite.setScale(m_sprite.getScale().x - 0.02f, m_sprite.getScale().y - 0.02f);
			m_text.setScale(m_sprite.getScale());
			m_image.setScale(m_sprite.getScale() * 2.0f);

			if (!m_imageButton)
			{
				m_sprite.rotate(0.5f * m_rotDirection);
				m_text.rotate(0.5f * m_rotDirection);
			}
		}
		// Animate out of hovered
		else if (!m_hovered && m_sprite.getScale().x < 1.0)
		{
			m_sprite.setScale(m_sprite.getScale().x + 0.02f, m_sprite.getScale().y + 0.02f);
			m_text.setScale(m_sprite.getScale());
			m_image.setScale(m_sprite.getScale() * 2.0f);

			if (!m_imageButton)
			{
				m_sprite.rotate(0.5f * -m_rotDirection);
				m_text.rotate(0.5f * -m_rotDirection);
			}
		}
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

	const sf::Vector2f Button::getPosition() const
	{
		return m_sprite.getPosition();
	}

	const sf::Vector2f Button::getSize() const
	{
		return { m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
	}
}