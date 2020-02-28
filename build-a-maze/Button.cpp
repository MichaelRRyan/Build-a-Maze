#include "Button.h"

namespace GUI
{
	////////////////////////////////////////////////////////////
	Button::Button(sf::Texture const& t_texture, sf::IntRect t_buttonRect, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position) :
		m_locked{ false },
		m_imageButton{ false },
		m_hovered{ false }
	{
		m_sprite.setTexture(t_texture);
		m_sprite.setTextureRect(t_buttonRect);
		m_sprite.setPosition(t_position.x + t_buttonRect.width / 2.0f, t_position.y + t_buttonRect.height / 2.0f);

		m_text.setPosition(t_position.x + t_buttonRect.width / 2.0f, t_position.y + t_buttonRect.height / 2.2f);
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
		m_hovered{ false },
		m_rotDirection{ 0.0f }
	{
		// Button background
		m_sprite.setTexture(t_guiTexture);
		m_sprite.setTextureRect(t_guiRect);
		m_sprite.setPosition(t_position.x + m_sprite.getGlobalBounds().width / 2.0f, t_position.y + m_sprite.getGlobalBounds().height / 2.0f);

		// Image overlay
		m_image.setTexture(t_imageTexture);
		m_image.setTextureRect(t_imageRect);
		m_image.setPosition(t_position.x + m_sprite.getGlobalBounds().width / 2.0f, t_position.y + m_sprite.getGlobalBounds().height / 2.0f);
	}

	////////////////////////////////////////////////////////////
	void Button::setup()
	{
		m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);
		m_text.setFillColor(sf::Color::Black);
		m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f, m_sprite.getGlobalBounds().height / 2.0f);

		m_image.setOrigin(m_image.getGlobalBounds().width / 2.0f, m_image.getGlobalBounds().height / 2.0f);
		m_image.setScale(s_IMAGE_OVERLAY_SCALE, s_IMAGE_OVERLAY_SCALE);
	}

	////////////////////////////////////////////////////////////
	bool Button::update(Cursor const& t_cursor)
	{
		animate();

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
	void Button::setOpacity(sf::Uint8 t_value)
	{
		m_sprite.setColor({ 255, 255, 255, t_value });
		m_image.setColor({ 255, 255, 255, t_value });

		if (!m_imageButton)
		{
			sf::Color color{ m_text.getFillColor() };
			color.a = t_value;
			m_text.setFillColor(color);

			color = m_text.getOutlineColor();
			color.a = t_value;
			m_text.setOutlineColor(color);
		}
	}

	////////////////////////////////////////////////////////////
	void Button::setLocked(bool t_state)
	{
		m_locked = t_state;

		if (t_state)
		{
			m_sprite.setColor(sf::Color{ 100, 100, 100 });
		}
		else
		{
			m_sprite.setColor(sf::Color::White);
		}
	}

	////////////////////////////////////////////////////////////
	bool Button::getLocked() const
	{
		return m_locked;
	}

	void Button::setCharacterSize(unsigned t_characterSize)
	{
		if (!m_imageButton)
		{
			m_text.setCharacterSize(t_characterSize);
		}
	}

	////////////////////////////////////////////////////////////
	void Button::setPosition(sf::Vector2f t_position)
	{
		// Reset the button animation before getting the size (effected by rotation)
		m_sprite.setRotation(0.0f);
		m_sprite.setScale(1.0f, 1.0f);
		
		// Set the position of all components
		m_sprite.setPosition(t_position.x + getSize().x / 2.0f, t_position.y + getSize().y / 2.0f);
		m_text.setPosition(t_position.x + getSize().x / 2.0f, t_position.y + getSize().y / 2.2f);
		m_image.setPosition(t_position.x + getSize().x / 2.0f, t_position.y + getSize().y / 2.0f);
	}

	////////////////////////////////////////////////////////////
	const sf::Vector2f Button::getPosition() const
	{
		return m_sprite.getPosition();
	}

	////////////////////////////////////////////////////////////
	const sf::Vector2f Button::getSize() const
	{
		return { m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
	}

	////////////////////////////////////////////////////////////
	void Button::animate()
	{
		// Animate to hovered
		if (m_hovered && m_sprite.getScale().x > 0.9)
		{
			m_sprite.setScale(m_sprite.getScale().x - 0.02f, m_sprite.getScale().y - 0.02f);
			m_text.setScale(m_sprite.getScale());
			m_image.setScale(m_sprite.getScale() * s_IMAGE_OVERLAY_SCALE);

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
			m_image.setScale(m_sprite.getScale() * s_IMAGE_OVERLAY_SCALE);

			if (!m_imageButton)
			{
				m_sprite.rotate(0.5f * -m_rotDirection);
				m_text.rotate(0.5f * -m_rotDirection);
			}
		}
	}

	////////////////////////////////////////////////////////////
	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite, states);

		if (!m_imageButton)
			target.draw(m_text, states);
		else
			target.draw(m_image, states);
	}
}