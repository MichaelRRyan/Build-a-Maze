#include "GUI.h"


/// <summary>
/// Default Constructor.
/// <para>Setup the object.</para>
/// </summary>
GUI::GUI()
{
	setupShopScreen();
	setupFontAndText();

	m_titleScreenBackground.setSize({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
	m_titleScreenBackground.setFillColor(sf::Color{ sf::Color{ 247, 230, 134 } });

	m_titleScreenButton.setPosition(PLAY_BUTTON_POSITION);
	m_titleScreenButton.setSize(PLAY_BUTTON_SIZE);
	m_titleScreenButton.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_titleScreenButton.setOutlineThickness(5.0f);
}

/// <summary>
/// Load the textures and setup all shape and sprite objects.
/// </summary>
void GUI::setupShopScreen()
{
	// Setup the shop background
	m_shopBackground.setPosition(SHOP_BACKGROUND_START_POS);
	m_shopBackground.setSize({ static_cast<float>(WINDOW_WIDTH) - SHOP_BACKGROUND_START_POS.x, static_cast<float>(WINDOW_HEIGHT) });
	m_shopBackground.setFillColor(sf::Color{ 247, 230, 134 });
	m_shopBackground.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopBackground.setOutlineThickness(5.0f);

	// Setup the shop item
	m_shopItem.setFillColor(sf::Color::White);
	m_shopItem.setSize(SHOP_ITEM_SIZE);
	m_shopItem.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopItem.setOutlineThickness(5.0f);

	// Load the terrain tiles texture
	if (!m_tilesTexture.loadFromFile("ASSETS/IMAGES/terrain_atlas.png"))
	{
		std::cout << "Error loading terrain textures";
	}

	if (!m_iconsTexture.loadFromFile("ASSETS/IMAGES/icons.png"))
	{
		std::cout << "Error loading icons textures";
	}

	// setup the shop item image sprite
	m_shopItemImage.setTexture(m_tilesTexture);
	m_shopItemImage.setScale(1.8f, 1.8f);

	m_iconsSprite.setTexture(m_iconsTexture);
	m_iconsSprite.setTextureRect(sf::IntRect{ 0,0,32,32 });
	m_iconsSprite.setScale(1.8f, 1.8f);
}

/// <summary>
/// Load the fonts and setup all text objects.
/// </summary>
void GUI::setupFontAndText()
{
	// Load the font
	if (!m_mainFont.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		std::cout << "Error loading main font (Arial)";
	}

	// Set the font for all text objects
	m_shopText.setFont(m_mainFont);
	m_priceText.setFont(m_mainFont);
	m_moneyText.setFont(m_mainFont);
	m_numAIText.setFont(m_mainFont);
	m_timeToCompleteText.setFont(m_mainFont);
	m_moneyEarnedText.setFont(m_mainFont);
	m_titleScreenButtonText.setFont(m_mainFont);
	m_titleText.setFont(m_mainFont);

	// Setup the shop text
	m_shopText.setString("CONSTRUCTION SHOP");
	m_shopText.setPosition(1050, 20);
	m_shopText.setFillColor(sf::Color::Black);
	m_shopText.setOrigin(m_shopText.getGlobalBounds().width / 2, 0.0f);

	// Setup the money text
	m_moneyText.setPosition(1050.0f, 80.0f);
	m_moneyText.setString("BALANCE: 400");
	m_moneyText.setFillColor(sf::Color{ 120, 112, 65 });
	m_moneyText.setOrigin(m_moneyText.getGlobalBounds().width / 2, 0.0f);

	// Setup the num AI text
	m_numAIText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING);
	m_numAIText.setFillColor(sf::Color{ 120, 112, 65 });

	// Setup the time to complete text
	m_timeToCompleteText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING + SIM_ICONS_SPACING);
	m_timeToCompleteText.setFillColor(sf::Color{ 120, 112, 65 });

	// Setup the money earned text
	m_moneyEarnedText.setPosition(SIM_ICONS_START_POS + SIM_ICONS_TEXT_SPACING + SIM_ICONS_SPACING * 2.0f);
	m_moneyEarnedText.setFillColor(sf::Color{ 120, 112, 65 });

	// Setup the title screen button text
	m_titleScreenButtonText.setFillColor(sf::Color::Black);
	m_titleScreenButtonText.setCharacterSize(40u);

	// Setup title text
	m_titleText.setString("Build-a-Maze!");
	m_titleText.setPosition(WINDOW_WIDTH / 2, 80.0f);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setCharacterSize(120u);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2, 0.0f);
	
}

/// <summary>
/// Draw the GUI for the constuction game state
/// </summary>
/// <param name="t_window">Render window</param>
void GUI::drawConstructionGUI(sf::RenderWindow & t_window)
{
	// Draw the panel background
	m_shopBackground.setPosition(GAMEPLAY_SECTION_END, 0.0f);
	t_window.draw(m_shopBackground);

	m_shopItem.setPosition(SHOP_DESTROY_TOOL_POS);
	m_shopItem.setFillColor(sf::Color::Red);
	t_window.draw(m_shopItem);
	m_shopItem.setFillColor(sf::Color::White);

	// Loop and draw the shop items
	m_shopItem.setPosition(SHOP_ITEM_START_POS);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < SHOP_ITEMS_PER_ROW; j++)
		{
			t_window.draw(m_shopItem);
			m_shopItem.move(SHOP_ITEM_SPACING.x, 0.0f);
		}

		m_shopItem.move(-SHOP_ITEM_SPACING.x * SHOP_ITEMS_PER_ROW, SHOP_ITEM_SPACING.y);
	}

	m_shopItemImage.setPosition(SHOP_ITEM_START_POS.x + 20, SHOP_ITEM_START_POS.y - 20);
	m_shopItemImage.setTextureRect(WALL_TEXT_RECT);
	t_window.draw(m_shopItemImage);

	m_shopItemImage.move(SHOP_ITEM_SPACING.x, 0.0f);
	m_shopItemImage.setTextureRect(PLANT_TEXT_RECT);
	t_window.draw(m_shopItemImage);

	t_window.draw(m_shopText);
	t_window.draw(m_moneyText);
}

/// <summary>
/// Draw the GUI for the simulation game state
/// </summary>
/// <param name="t_window"></param>
/// <param name="t_noOfAI"></param>
/// <param name="t_timeToComplete"></param>
/// <param name="t_moneyEarned"></param>
void GUI::drawSimulationGUI(sf::RenderWindow & t_window, int t_noOfAI, float t_timeToComplete, int t_moneyEarned)
{
	// Draw the panel background
	m_shopBackground.setPosition(SIM_PANEL_START, 0.0f);

	t_window.draw(m_shopBackground);

	// Work out minutes and seconds and set the string
	int seconds = static_cast<int>(floor(t_timeToComplete)) % 60;
	int minutes = static_cast<int>(floor(t_timeToComplete)) / 60;

	m_numAIText.setString(std::to_string(t_noOfAI) + " / " + std::to_string(BASIC_SOLVERS_MAX));
	
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

/// <summary>
/// Draw the title screen
/// </summary>
/// <param name="t_window"></param>
void GUI::drawTitleScreen(sf::RenderWindow & t_window)
{
	t_window.draw(m_titleScreenBackground);

	t_window.draw(m_titleText);

	m_titleScreenButton.setPosition(PLAY_BUTTON_POSITION);
	m_titleScreenButtonText.setPosition(PLAY_BUTTON_POSITION.x + PLAY_BUTTON_SIZE.x / 2.0f, PLAY_BUTTON_POSITION.y + PLAY_BUTTON_SIZE.y / 2.0f - 10.0f);
	for (int i = 0; i < 3; i++)
	{
		t_window.draw(m_titleScreenButton);
		m_titleScreenButton.move(0.0f, PLAY_BUTTON_SIZE.y * 1.5f);

		if (i == 0)
		{
			m_titleScreenButtonText.setString("START");
		}
		else if (i == 1)
		{
			m_titleScreenButtonText.setString("SETTINGS");
		}
		else
		{
			m_titleScreenButtonText.setString("EXIT");
		}
		
		m_titleScreenButtonText.setOrigin(m_titleScreenButtonText.getGlobalBounds().width / 2, m_titleScreenButtonText.getGlobalBounds().height / 2);
		t_window.draw(m_titleScreenButtonText);
		m_titleScreenButtonText.move(0.0f, PLAY_BUTTON_SIZE.y * 1.5f);
	}
}

void GUI::processTitleEvents(sf::Event t_event, GameState &t_gameState, bool &t_exitGame)
{
	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		if (sf::Mouse::Left == t_event.mouseButton.button)
		{
			if (t_event.mouseButton.x > PLAY_BUTTON_POSITION.x
				&& t_event.mouseButton.x < PLAY_BUTTON_POSITION.x + PLAY_BUTTON_SIZE.x)
			{
				if (t_event.mouseButton.y > PLAY_BUTTON_POSITION.y
					&& t_event.mouseButton.y < PLAY_BUTTON_POSITION.y + PLAY_BUTTON_SIZE.y)
				{
					t_gameState = GameState::BuildMode;
				}

				if (t_event.mouseButton.y > PLAY_BUTTON_POSITION.y + (PLAY_BUTTON_SIZE.y * 3)
					&& t_event.mouseButton.y < PLAY_BUTTON_POSITION.y + PLAY_BUTTON_SIZE.y + (PLAY_BUTTON_SIZE.y * 3))
				{
					t_exitGame = true;
				}
			}
		}
	}
}

void GUI::processEvents(sf::Event t_event, ConstructionMode & t_constructionState, TileType &t_selectedTileType)
{
	// Check for a mouse click event
	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		if (sf::Mouse::Left == t_event.mouseButton.button)
		{
			// Wall item
			if (t_event.mouseButton.x > SHOP_ITEM_START_POS.x && t_event.mouseButton.x < SHOP_ITEM_START_POS.x + SHOP_ITEM_SIZE.x
				&& t_event.mouseButton.y > SHOP_ITEM_START_POS.y && t_event.mouseButton.y < SHOP_ITEM_START_POS.y + SHOP_ITEM_SIZE.y)
			{
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::Wall;
				std::cout << "Mode Switched to 'Placing' the 'Wall' tile" << std::endl;
			}

			if (t_event.mouseButton.x > SHOP_ITEM_START_POS.x + SHOP_ITEM_SPACING.x && t_event.mouseButton.x < SHOP_ITEM_START_POS.x + SHOP_ITEM_SPACING.x + SHOP_ITEM_SIZE.x
				&& t_event.mouseButton.y > SHOP_ITEM_START_POS.y && t_event.mouseButton.y < SHOP_ITEM_START_POS.y + SHOP_ITEM_SIZE.y)
			{
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::Slow;
				std::cout << "Mode Switched to 'Placing' the 'Plant' tile" << std::endl;
			}

			// Destroy tool
			if (t_event.mouseButton.x > SHOP_DESTROY_TOOL_POS.x && t_event.mouseButton.x < SHOP_DESTROY_TOOL_POS.x + SHOP_ITEM_SIZE.x
				&& t_event.mouseButton.y > SHOP_DESTROY_TOOL_POS.y && t_event.mouseButton.y < SHOP_DESTROY_TOOL_POS.y + SHOP_ITEM_SIZE.y)
			{
				t_constructionState = ConstructionMode::Destroying;
				std::cout << "Mode Switched to 'Destroying'" << std::endl;
			}
		}
	}
	if (sf::Event::KeyPressed == t_event.type)
	{
		if (sf::Keyboard::Escape == t_event.key.code)
		{
			t_constructionState = ConstructionMode::None;
			t_selectedTileType = TileType::None;
			std::cout << "Mode Switched to 'None'" << std::endl;
		}
	}
}

void GUI::update(sf::Vector2i t_mousePosition, int & t_money)
{
	// Set the money string
	m_moneyText.setString("BALANCE: " + std::to_string(t_money));

	//if (t_mousePosition.x > )
}
