#include "Popup.h"

GUI::Popup::Popup(sf::Vector2f t_position, std::string t_message, std::string t_secondaryMessage) :
	m_okButton(m_texture, { 65, 81, 100, 40 }, m_font, "OK", { t_position.x + 100.0f, t_position.y + 150.0f }),
	m_active{ false }
{
	// Load the button texure
	if (!m_texture.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	// Load the button font
	if (!m_font.loadFromFile("ASSETS/FONTS/tf2Build.ttf"))
	{
		std::cout << "Error loading popup font";
	}

	m_okButton.setCharacterSize(20u);
	m_okButton.setup();

	m_backgroundShape.setSize({ 300.0f, 200.0f });
	m_backgroundShape.setFillColor(sf::Color{ 247, 230, 134 });
	m_backgroundShape.setPosition(t_position);
	m_backgroundShape.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_backgroundShape.setOutlineThickness(5.0f);

	setupText(m_messageText, t_message, t_position, sf::Color::Black);
	setupText(m_secondaryMessageText, t_secondaryMessage, { t_position.x, m_messageText.back().getPosition().y + m_messageText.back().getGlobalBounds().height }, sf::Color{ 100, 92, 45 });
}

void GUI::Popup::update(Cursor const& t_cursor)
{
	if (m_active)
	{
		if (m_okButton.update(t_cursor))
		{
			m_active = false;
		}
	}
}

void GUI::Popup::setActive(bool t_active)
{
	m_active = t_active;
}

const bool GUI::Popup::isActive() const
{
	return m_active;
}

void GUI::Popup::draw(sf::RenderWindow& t_window)
{
	if (m_active)
	{
		sf::RectangleShape shadow;
		float thickness = m_backgroundShape.getOutlineThickness() * 2.0f;
		shadow.setPosition(m_backgroundShape.getPosition().x + m_backgroundShape.getSize().x / 2.0f, m_backgroundShape.getPosition().y + m_backgroundShape.getSize().y / 2.0f);

		shadow.setFillColor(sf::Color{ 10, 10, 10, 10 });

		for (int i = 0; i < 20; i++)
		{
			shadow.setSize({ m_backgroundShape.getSize().x + thickness + i, m_backgroundShape.getSize().y + thickness + i });
			shadow.setOrigin(shadow.getSize().x / 2.0f, shadow.getSize().y / 2.0f);
			
			t_window.draw(shadow);
		}

		t_window.draw(m_backgroundShape);
		t_window.draw(m_okButton);

		for (sf::Text const& text : m_messageText)
		{
			t_window.draw(text);
		}

		for (sf::Text const& text : m_secondaryMessageText)
		{
			t_window.draw(text);
		}
	}
}

void GUI::Popup::setupText(std::vector<sf::Text>& t_text, std::string t_string, sf::Vector2f t_position, sf::Color t_color)
{
	if (t_string != "")
	{
		std::string line;
		sf::Text lineText("", m_font, 20u);
		lineText.setPosition(t_position.x + 150.0f, t_position.y + 10.0f);
		lineText.setFillColor(t_color);

		for (int i = 0; i < t_string.length(); i++)
		{
			if (t_string.at(i) == '\n')
			{
				lineText.setString(line);
				lineText.setOrigin(lineText.getGlobalBounds().width / 2.0f, 0.0f);

				t_text.push_back(lineText);

				line.clear();
				lineText.move(0.0f, 25.0f);
			}
			else
			{
				line += t_string.at(i);
			}
		}

		if (line != "")
		{
			lineText.setString(line);
			lineText.setOrigin(lineText.getGlobalBounds().width / 2.0f, 0.0f);

			t_text.push_back(lineText);
		}
	}
}