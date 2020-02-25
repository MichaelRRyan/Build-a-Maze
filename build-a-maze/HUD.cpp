#include "HUD.h"

/////////////////////////////////////////////////////////////////
HUD::HUD(sf::View const& t_windowView) :
	m_shopItems{ {
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 96, 64, 16, 16 }, { t_windowView.getSize().x / 1.5f + 96.0f, t_windowView.getSize().y / 2.0f - 110.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 0, 88, 16, 24 }, { t_windowView.getSize().x / 1.5f, t_windowView.getSize().y / 2.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, MUD_TEXT_RECT,{ t_windowView.getSize().x / 1.5f + 96.0f, t_windowView.getSize().y / 2.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 16, 0, 16, 16 }, { t_windowView.getSize().x / 1.5f + 192.0f, t_windowView.getSize().y / 2.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 0, 112, 16, 16 }, { t_windowView.getSize().x / 1.5f, t_windowView.getSize().y / 2.0f + 110.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 0, 128, 16, 16 }, { t_windowView.getSize().x / 1.5f + 96.0f, t_windowView.getSize().y / 2.0f + 110.0f } },
		{ m_guiTextures, m_tileTextures, { 0, 80, 64, 64 }, { 16, 96, 16, 16 }, { t_windowView.getSize().x / 1.5f + 192.0f, t_windowView.getSize().y / 2.0f + 110.0f } }
	} },
	m_playButton(m_guiTextures, m_tileTextures, { 0,147,32,32 }, { 112, 64, 16, 16 }, { t_windowView.getSize().x - (t_windowView.getSize().x / 2.6f) - 20.0f, t_windowView.getSize().y / 2.0f - 16.0f }),
	m_stopButton(m_guiTextures, m_tileTextures, { 0,147,32,32 }, { 128, 64, 16, 16 }, { t_windowView.getSize().x - (t_windowView.getSize().x / 2.6f) - 20.0f, t_windowView.getSize().y / 2.0f - 16.0f })
{
	m_shopBackground.setSize({ t_windowView.getSize().x / 2.7f, t_windowView.getSize().y });
	m_shopBackground.setFillColor(sf::Color{ 247, 230, 134 });
	m_shopBackground.setPosition(t_windowView.getSize().x - m_shopBackground.getSize().x, 0.0f);

	m_shopBackground.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_shopBackground.setOutlineThickness(5.0f);

	if (!m_guiTextures.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		std::cout << "Error loading tile sheet textures";
	}

	for (GUI::Button & button : m_shopItems)
	{
		button.setup();
	}

	m_playButton.setup();
	m_stopButton.setup();

	// Load the font
	if (!m_hudFont.loadFromFile("ASSETS/FONTS/tf2Build.ttf"))
	{
		std::cout << "Error loading main font (tf2Build)";
	}

	float shopCentre = m_shopBackground.getPosition().x + m_shopBackground.getSize().x / 2.0f;

	m_menuTab.setSize({ 60.0f, 100.0f });
	m_menuTab.setFillColor(sf::Color{ 247, 230, 134 });
	m_menuTab.setPosition(t_windowView.getSize().x - m_shopBackground.getSize().x - m_menuTab.getGlobalBounds().width * 0.8f, (t_windowView.getSize().y / 2.0f) - m_menuTab.getGlobalBounds().height / 2.0f);

	m_menuTab.setOutlineThickness(5.0f);
	m_menuTab.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_menuTab.setCornerRadius(15.0f);
	

	// Setup the shop text
	m_shopTitleText.setFont(m_hudFont);
	m_shopTitleText.setString("CONSTRUCTION SHOP");
	m_shopTitleText.setPosition(shopCentre, t_windowView.getSize().y * 0.1f);
	m_shopTitleText.setFillColor(sf::Color::Black);
	m_shopTitleText.setOrigin(m_shopTitleText.getGlobalBounds().width / 2, 0.0f);

	// Setup the money text
	m_moneyText.setFont(m_hudFont);
	m_moneyText.setPosition(shopCentre, t_windowView.getSize().y * 0.2f);
	m_moneyText.setString("BALANCE: 400");
	m_moneyText.setFillColor(sf::Color{ 120, 112, 65 });
	m_moneyText.setOrigin(m_moneyText.getGlobalBounds().width / 2, 0.0f);

	// Stats panel
	m_statsBackground.setSize({ t_windowView.getSize().x / 2.7f, t_windowView.getSize().y });
	m_statsBackground.setFillColor(sf::Color{ 247, 230, 134 });
	m_statsBackground.setPosition(t_windowView.getSize().x - m_shopBackground.getSize().x, 0.0f);

	m_statsBackground.setOutlineColor(sf::Color{ 120, 112, 65 });
	m_statsBackground.setOutlineThickness(5.0f);

	//m_statsBackground.setSize({ t_windowView.getSize().x * 0.2f, t_windowView.getSize().y });
	//m_statsBackground.setPosition(t_windowView.getSize().x - m_statsBackground.getSize().x, 0.0f);
	
	m_iconsPosition = { m_statsBackground.getPosition().x + m_statsBackground.getSize().x * 0.2f, t_windowView.getSize().y * 0.4f };

	if (!m_iconTextures.loadFromFile("ASSETS/IMAGES/icons.png"))
	{
		std::cout << "Error loading icons textures";
	}

	m_iconSprite.setTexture(m_iconTextures);

	m_numAIText.setFont(m_hudFont);
	m_moneyEarnedText.setFont(m_hudFont);
	m_timeText.setFont(m_hudFont);

	m_numAIText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y);
	m_timeText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y + 32.0f * 1.2f);
	m_moneyEarnedText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y + 32.0f * 1.2f * 2.0f);

	m_numAIText.setFillColor(sf::Color{ 120, 112, 65 });
	m_moneyEarnedText.setFillColor(sf::Color{ 120, 112, 65 });
	m_timeText.setFillColor(sf::Color{ 120, 112, 65 });

	m_numAIText.setCharacterSize(20.0f);
	m_moneyEarnedText.setCharacterSize(20.0f);
	m_timeText.setCharacterSize(20.0f);

	// Setup shop item names
	m_shopItemNames.at(0).setString("Destroy Tool");
	m_shopItemNames.at(1).setString("Wall");
	m_shopItemNames.at(2).setString("Mud");
	m_shopItemNames.at(3).setString("Treadmill");
	m_shopItemNames.at(4).setString("Balancing\n    Pad");
	m_shopItemNames.at(5).setString("Paintball\n  Turret");
	m_shopItemNames.at(6).setString("Trapdoor");

	m_shopItemPrices.at(0).setString("$" + std::to_string(Global::getTilePrice(TileType::Wall))); // Wall
	m_shopItemPrices.at(1).setString("$" + std::to_string(Global::getTilePrice(TileType::Mud))); // Mud
	m_shopItemPrices.at(2).setString("$" + std::to_string(Global::getTilePrice(TileType::TreadmillWest))); // Treadmill
	m_shopItemPrices.at(3).setString("$" + std::to_string(Global::getTilePrice(TileType::SteppingStones))); // Balancing pad
	m_shopItemPrices.at(4).setString("$" + std::to_string(Global::getTilePrice(TileType::TurretWest))); // Turret
	m_shopItemPrices.at(5).setString("$" + std::to_string(Global::getTilePrice(TileType::Trapdoor))); // Trapdoor

	for (int i = 0; i < m_shopItems.size(); i++)
	{
		m_shopItemNames.at(i).setFont(m_hudFont);
		m_shopItemNames.at(i).setFillColor(sf::Color::Black);
		m_shopItemNames.at(i).setCharacterSize(13u);
		m_shopItemNames.at(i).setOrigin(m_shopItemNames.at(i).getGlobalBounds().width / 2.0f, 0.0f);
		m_shopItemNames.at(i).setPosition(m_shopItems.at(i).getPosition().x, m_shopItems.at(i).getPosition().y + m_shopItems.at(i).getSize().y / 2.0f);
	}

	for (int i = 0; i < m_shopItemPrices.size(); i++)
	{
		m_shopItemPrices.at(i).setFont(m_hudFont);
		m_shopItemPrices.at(i).setFillColor(sf::Color{ 120, 112, 65 });
		m_shopItemPrices.at(i).setCharacterSize(13u);
		m_shopItemPrices.at(i).setOrigin(m_shopItemPrices.at(i).getGlobalBounds().width / 2.0f, 0.0f);
		m_shopItemPrices.at(i).setPosition(m_shopItemNames.at(i + 1).getPosition().x, m_shopItemNames.at(i + 1).getPosition().y + m_shopItemNames.at(i + 1).getGlobalBounds().height + 5);
	}
}

/////////////////////////////////////////////////////////////////
void HUD::updateBuildMode(Cursor t_cursor, Game* t_game, std::function<void(Game*)> t_func, ConstructionMode& t_constructionState, TileType& t_selectedTileType, int t_money)
{
	m_moneyText.setString("BALANCE: $" + std::to_string(t_money));

	for (int i = 0; i < m_shopItems.size(); i++)
	{
		if (m_shopItems.at(i).update(t_cursor))
		{
			switch (i)
			{
			case 0: // Destroy tool
				t_constructionState = ConstructionMode::Destroying;
				t_selectedTileType = TileType::None;
				break;
			case 1: // Wall tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::Wall;
				break;
			case 2: // Mud tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::Mud;
				break;
			case 3: // Treadmill tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::TreadmillWest;
				break;
			case 4: // Stepping Stones tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::SteppingStones;
				break;
			case 5: // Stepping Stones tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::TurretWest;
				break;
			case 6: // Stepping Stones tool
				t_constructionState = ConstructionMode::Placing;
				t_selectedTileType = TileType::Trapdoor;
				break;
			}
		}
	}

	if (t_cursor.m_cancelClicked)
	{
		t_constructionState = ConstructionMode::None;
		t_selectedTileType = TileType::None;
	}

	if (m_playButton.update(t_cursor))
	{
		t_func(t_game);
	}
}

/////////////////////////////////////////////////////////////////
void HUD::updateSimText(Cursor t_cursor, Game* t_game, std::function<void(Game*)> t_func, int t_noOfAI, float t_timeToComplete, int t_moneyEarned)
{
	// Work out minutes and seconds and set the string
	int seconds = static_cast<int>(floor(t_timeToComplete)) % 60;
	int minutes = static_cast<int>(floor(t_timeToComplete)) / 60;

	if (seconds < 10) // if below 10 seconds, add a zero before the seconds as to display correctly as a time
	{
		m_timeText.setString(std::to_string(minutes) + ":0" + std::to_string(seconds));
	}
	else
	{
		m_timeText.setString(std::to_string(minutes) + ":" + std::to_string(seconds));
	}

	m_numAIText.setString(std::to_string(t_noOfAI) + " / " + std::to_string(SOLVERS_MAX));

	m_moneyEarnedText.setString(std::to_string(t_moneyEarned));

	if (m_stopButton.update(t_cursor))
	{
		t_func(t_game);
	}
}

/////////////////////////////////////////////////////////////////
void HUD::drawShop(sf::RenderWindow& t_window) const
{
	sf::RoundedRectangleShape shape{ m_menuTab };

	t_window.draw(shape);

	t_window.draw(m_shopBackground);

	shape.setOutlineThickness(0.0f);
	t_window.draw(shape);

	for (int i = 0; i < m_shopItems.size(); i++)
	{
		t_window.draw(m_shopItems.at(i));
		t_window.draw(m_shopItemNames.at(i));

		if (i < m_shopItemPrices.size())
		{
			t_window.draw(m_shopItemPrices.at(i));
		}
	}

	t_window.draw(m_shopTitleText);
	t_window.draw(m_moneyText);
	t_window.draw(m_playButton);
}

/////////////////////////////////////////////////////////////////
void HUD::drawStats(sf::RenderWindow& t_window)
{
	sf::RoundedRectangleShape shape{ m_menuTab };

	t_window.draw(shape);

	t_window.draw(m_statsBackground);

	shape.setOutlineThickness(0.0f);
	t_window.draw(shape);

	t_window.draw(m_numAIText);
	t_window.draw(m_moneyEarnedText);
	t_window.draw(m_timeText);

	// Draw the icons
	for (int i = 0; i < 3; i++)
	{
		m_iconSprite.setPosition( m_iconsPosition.x, m_iconsPosition.y + m_iconSprite.getGlobalBounds().height * 1.2f * i );
		m_iconSprite.setTextureRect(sf::IntRect{ 32 * i, 0, 32, 32 });
		t_window.draw(m_iconSprite);
	}

	t_window.draw(m_stopButton);
}

/////////////////////////////////////////////////////////////////