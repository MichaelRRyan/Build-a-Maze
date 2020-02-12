#include "GUI.h"


/// <summary>
/// Default Constructor.
/// <para>Setup the object.</para>
/// </summary>
Screens::Screens()
{
	if (!m_iconsTexture.loadFromFile("ASSETS/IMAGES/icons.png"))
	{
		std::cout << "Error loading icons textures";
	}

	if (!m_guiTextures.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	m_iconsSprite.setTexture(m_iconsTexture);
	m_iconsSprite.setTextureRect(sf::IntRect{ 0,0,32,32 });
	m_iconsSprite.setScale(1.8f, 1.8f);

	setupFontAndText();
}

/// <summary>
/// Load the fonts and setup all text objects.
/// </summary>
void Screens::setupFontAndText()
{
	// Load the font
	if (!m_mainFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	m_numAIText.setFont(m_mainFont);
	m_timeToCompleteText.setFont(m_mainFont);
	m_moneyEarnedText.setFont(m_mainFont);

	// Setup the num AI text
	m_numAIText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING);
	m_numAIText.setFillColor(sf::Color{ 120, 112, 65 });

	// Setup the time to complete text
	m_timeToCompleteText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING + SIM_ICONS_SPACING);
	m_timeToCompleteText.setFillColor(sf::Color{ 120, 112, 65 });

	// Setup the money earned text
	m_moneyEarnedText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING + SIM_ICONS_SPACING * 2.0f);
	m_moneyEarnedText.setFillColor(sf::Color{ 120, 112, 65 });
}

/// <summary>
/// Draw the GUI for the simulation game state
/// </summary>
/// <param name="t_window"></param>
/// <param name="t_noOfAI"></param>
/// <param name="t_timeToComplete"></param>
/// <param name="t_moneyEarned"></param>
void Screens::drawSimulationGUI(sf::RenderWindow & t_window, int t_noOfAI, float t_timeToComplete, int t_moneyEarned)
{
	// Work out minutes and seconds and set the string
	int seconds = static_cast<int>(floor(t_timeToComplete)) % 60;
	int minutes = static_cast<int>(floor(t_timeToComplete)) / 60;

	m_numAIText.setString(std::to_string(t_noOfAI) + " / " + std::to_string(SOLVERS_MAX));
	
	// Draw the icons
	m_iconsSprite.setPosition(SIM_ICONS_START_POS);
	for (int i = 0; i < 3; i++)
	{
		// Money earned icon
		m_iconsSprite.setTextureRect(sf::IntRect{ 32 * i,0,32,32 });
		t_window.draw(m_iconsSprite);
		m_iconsSprite.move(SIM_ICONS_SPACING);
	}

	if (seconds < 10) // if below 10 seconds, add a zero before the seconds as to display correctly as a time
	{
		m_timeToCompleteText.setString(std::to_string(minutes) + ":0" + std::to_string(seconds));
	}
	else
	{
		m_timeToCompleteText.setString(std::to_string(minutes) + ":" + std::to_string(seconds));
	}

	m_moneyEarnedText.setString(std::to_string(t_moneyEarned));

	t_window.draw(m_numAIText);
	t_window.draw(m_moneyEarnedText);
	t_window.draw(m_timeToCompleteText);
}
