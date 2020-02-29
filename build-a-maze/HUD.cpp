#include "HUD.h"

/////////////////////////////////////////////////////////////////
HUD::HUD(sf::View const& t_windowView, MazeEditor& t_mazeEditor, Game * t_game) :
	m_SHOP_ITEM_RECT{ 0, 80, 48, 48 },
	m_mainColor{ 247, 230, 134 },
	m_secondaryColor{ 120, 112, 65 },
	m_secondaryTextColor{ 100, 92, 45 },
	m_VERTICAL_BUTTON_OFFSETS{ t_windowView.getSize().y / 2.0f - 50.0f,
								t_windowView.getSize().y / 2.0f + 60.0f,
								t_windowView.getSize().y / 2.0f + 150.0f },
	m_shopItems{ {
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 64, 0, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 96, 64, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 80, 0, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 0, 88, 16, 24 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, MUD_TEXT_RECT },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 16, 0, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 0, 112, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 0, 128, 16, 16 } },
		{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, { 16, 96, 16, 16 } }
	} },
	m_playButton(m_guiTextures, m_tileTextures, { 0,147,32,32 }, { 96, 0, 16, 16 }, { t_windowView.getSize().x - (t_windowView.getSize().x / 3.3f) - 20.0f, t_windowView.getSize().y / 2.0f - 16.0f }),
	m_stopButton(m_guiTextures, m_tileTextures, { 0,147,32,32 }, { 112, 0, 16, 16 }, { t_windowView.getSize().x - (t_windowView.getSize().x / 4.6f) - 20.0f, t_windowView.getSize().y / 2.0f + 4.0f }),
	m_pauseButton(m_guiTextures, m_tileTextures, { 0,147,32,32 }, { 128, 0, 16, 16 }, { t_windowView.getSize().x - (t_windowView.getSize().x / 4.6f) - 20.0f, t_windowView.getSize().y / 2.0f - 36.0f }),
	m_sheepButton{ m_guiTextures, m_tileTextures, m_SHOP_ITEM_RECT, {112, 64, 16, 17} },
	m_mazeEditorRef{ t_mazeEditor },
	m_SECONDS_TO_ANIMATE{ 0.2f },
	m_animationState{ AnimationState::None },
	m_gamePtr{ t_game }
{
	loadFiles();
	setupShopMenu(t_windowView);
	setupStatsMenu(t_windowView);
}

/////////////////////////////////////////////////////////////////
void HUD::updateBuildMode(Cursor t_cursor, std::function<void(Game*)> t_playButtonFunc, std::function<void(Game*)> t_purchaseSheepFunc, int t_money)
{
	m_moneyText.setString("BALANCE: $" + std::to_string(t_money));

	
	if (AnimationState::ShopIn == m_animationState)
	{
		if (m_animationClock.getElapsedTime().asSeconds() >= m_SECONDS_TO_ANIMATE)
		{
			m_animationState = AnimationState::None;
		}
	}
	else // Button functionality should not work when animating
	{
		for (unsigned i = 0; i < m_shopItems.size(); i++)
		{
			if (m_shopItems.at(i).update(t_cursor))
			{
				switch (i)
				{
				case 0: // Undo
					m_mazeEditorRef.undoAction();
					break;
				case 1: // Destroy tool
					m_mazeEditorRef.enableDestroyMode();
					break;
				case 2: // Redo
					m_mazeEditorRef.redoAction();
					break;
				case 3: // Wall tool
					m_mazeEditorRef.setSelectedTileType(TileType::Wall);
					break;
				case 4: // Mud tool
					m_mazeEditorRef.setSelectedTileType(TileType::Mud);
					break;
				case 5: // Treadmill tool
					m_mazeEditorRef.setSelectedTileType(TileType::TreadmillWest);
					break;
				case 6: // Stepping Stones tool
					m_mazeEditorRef.setSelectedTileType(TileType::SteppingStones);
					break;
				case 7: // Turret tool
					m_mazeEditorRef.setSelectedTileType(TileType::TurretWest);
					break;
				case 8: // Trapdoor tool
					m_mazeEditorRef.setSelectedTileType(TileType::Trapdoor);
					break;
				}
			}
		}

		if (m_sheepButton.update(t_cursor))
		{
			t_purchaseSheepFunc(m_gamePtr);
		}

		if (m_playButton.update(t_cursor))
		{
			t_playButtonFunc(m_gamePtr);
		}
	}

	if (t_cursor.m_cancelClicked)
	{
		m_mazeEditorRef.unselectEditTool();
	}
}

/////////////////////////////////////////////////////////////////
void HUD::updateSimText(Cursor t_cursor, std::function<void(Game*)> t_stopButtonFunc, std::function<void(Game*)> t_pauseButtonFunc, int t_maxAI, int t_noOfAI, float t_timeToComplete, int t_moneyEarned)
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

	m_numAIText.setString(std::to_string(t_noOfAI) + " / " + std::to_string(t_maxAI));

	m_moneyEarnedText.setString(std::to_string(t_moneyEarned));

	if (AnimationState::StatsIn == m_animationState)
	{
		if (m_animationClock.getElapsedTime().asSeconds() >= m_SECONDS_TO_ANIMATE)
		{
			m_animationState = AnimationState::None;
		}
	}
	else // Button functionality should not work when animating
	{
		if (m_stopButton.update(t_cursor))
		{
			t_stopButtonFunc(m_gamePtr);
		}

		if (m_pauseButton.update(t_cursor))
		{
			t_pauseButtonFunc(m_gamePtr);
		}
	}
}

/////////////////////////////////////////////////////////////////
void HUD::drawShop(sf::RenderWindow& t_window)
{
	// Get the current view
	sf::View view{ t_window.getView() };

	// If animating in
	if (AnimationState::ShopIn == m_animationState)
	{
		drawStats(t_window);

		float AnimationCompleteness{ m_animationClock.getElapsedTime().asSeconds() / m_SECONDS_TO_ANIMATE };
		float animInOffset{ -m_shopBackground.getSize().x - (1.0f - AnimationCompleteness * m_shopBackground.getSize().x) };

		sf::View modifiedView{ view };
		modifiedView.move(animInOffset, 0.0f);
		t_window.setView(modifiedView);
	}
	if (AnimationState::StatsIn == m_animationState)
	{
		float AnimationCompleteness{ m_animationClock.getElapsedTime().asSeconds() / m_SECONDS_TO_ANIMATE };
		float animInOffset{ (1.0f - AnimationCompleteness * m_shopBackground.getSize().x) };

		sf::View modifiedView{ view };
		modifiedView.move(animInOffset, 0.0f);
		t_window.setView(modifiedView);
	}

	sf::RoundedRectangleShape shape{ m_menuTab };

	shape.setPosition(m_shopBackground.getPosition().x - m_menuTab.getGlobalBounds().width * 0.7f,
		(m_shopBackground.getGlobalBounds().height / 2.0f) - m_menuTab.getGlobalBounds().height / 2.0f);

	t_window.draw(shape);

	t_window.draw(m_shopBackground);

	t_window.draw(m_shopDividers[0]);
	t_window.draw(m_shopDividers[1]);

	shape.setOutlineThickness(0.0f);
	t_window.draw(shape);

	for (unsigned i = 0; i < m_shopItems.size(); i++)
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

	t_window.draw(m_sheepButton);
	t_window.draw(m_sheepText);
	t_window.draw(m_sheepPrice);

	// Reset the view to what it was before this function call
	t_window.setView(view);
}

/////////////////////////////////////////////////////////////////
void HUD::drawStats(sf::RenderWindow& t_window)
{
	// Get the current view
	sf::View view{ t_window.getView() };

	// If animating in
	if (AnimationState::StatsIn == m_animationState)
	{
		drawShop(t_window);

		float AnimationCompleteness{ m_animationClock.getElapsedTime().asSeconds() / m_SECONDS_TO_ANIMATE };
		float animInOffset{ -m_shopBackground.getSize().x - (1.0f - AnimationCompleteness * m_shopBackground.getSize().x) };

		sf::View modifiedView{ view };
		modifiedView.move(animInOffset, 0.0f);
		t_window.setView(modifiedView);
	}
	else if (AnimationState::ShopIn == m_animationState)
	{
		float AnimationCompleteness{ m_animationClock.getElapsedTime().asSeconds() / m_SECONDS_TO_ANIMATE };
		float animInOffset{ ( 1.0f - AnimationCompleteness * m_shopBackground.getSize().x) };

		sf::View modifiedView{ view };
		modifiedView.move(animInOffset, 0.0f);
		t_window.setView(modifiedView);
	}

	sf::RoundedRectangleShape shape{ m_menuTab };

	shape.setPosition(m_statsBackground.getPosition().x - m_menuTab.getGlobalBounds().width * 0.7f,
		(m_statsBackground.getGlobalBounds().height / 2.0f) - m_menuTab.getGlobalBounds().height / 2.0f);

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
	t_window.draw(m_pauseButton);

	// Reset the view to what it was before this function call
	t_window.setView(view);
}

/////////////////////////////////////////////////////////////////
void HUD::animateInShop()
{
	m_animationState = AnimationState::ShopIn;
	m_animationClock.restart();
}

/////////////////////////////////////////////////////////////////
void HUD::animateInStats()
{
	m_animationState = AnimationState::StatsIn;
	m_animationClock.restart();
}

/////////////////////////////////////////////////////////////////
const float HUD::getSecondsToAnimate() const
{
	return m_SECONDS_TO_ANIMATE;
}

/////////////////////////////////////////////////////////////////
void HUD::loadFiles()
{
	// Load the GUI textures
	if (!m_guiTextures.loadFromFile("ASSETS/IMAGES/GUI.png"))
	{
		std::cout << "Error loading GUI textures";
	}

	// Load the tile textures
	if (!m_tileTextures.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		std::cout << "Error loading tile sheet textures";
	}

	// Load the in-game icon textures
	if (!m_iconTextures.loadFromFile("ASSETS/IMAGES/icons.png"))
	{
		std::cout << "Error loading icons textures";
	}

	// Load the font
	if (!m_hudFont.loadFromFile("ASSETS/FONTS/tf2Build.ttf"))
	{
		std::cout << "Error loading main font (tf2Build)";
	}
}

/////////////////////////////////////////////////////////////////
void HUD::setupShopMenu(sf::View const& t_windowView)
{
	// Setup the shop background
	m_shopBackground.setSize({ t_windowView.getSize().x / 3.5f, t_windowView.getSize().y });
	m_shopBackground.setFillColor(m_mainColor);
	m_shopBackground.setPosition(t_windowView.getSize().x - m_shopBackground.getSize().x, 0.0f);
	m_shopBackground.setOutlineColor(m_secondaryColor);
	m_shopBackground.setOutlineThickness(5.0f);

	// Find the centre of the shop background
	float shopCentre = m_shopBackground.getPosition().x + m_shopBackground.getSize().x / 2.0f;

	// Setup the menu tab (Tab at the side of the menu to hold play/stop buttons)
	m_menuTab.setSize({ 60.0f, 100.0f });
	m_menuTab.setFillColor(m_mainColor);
	m_menuTab.setPosition(t_windowView.getSize().x - m_shopBackground.getSize().x - m_menuTab.getGlobalBounds().width * 0.8f,
		(t_windowView.getSize().y / 2.0f) - m_menuTab.getGlobalBounds().height / 2.0f);

	m_menuTab.setOutlineThickness(5.0f);
	m_menuTab.setOutlineColor(m_secondaryColor);
	m_menuTab.setCornerRadius(15.0f);

	// Setup the shop text
	m_shopTitleText.setFont(m_hudFont);
	m_shopTitleText.setString("Farmers Market");
	m_shopTitleText.setCharacterSize(22u);
	m_shopTitleText.setPosition(shopCentre, t_windowView.getSize().y * 0.05f);
	m_shopTitleText.setFillColor(sf::Color::Black);
	m_shopTitleText.setOrigin(m_shopTitleText.getGlobalBounds().width / 2, 0.0f);
	

	// Setup the money text
	m_moneyText.setFont(m_hudFont);
	m_moneyText.setCharacterSize(20u);
	m_moneyText.setPosition(shopCentre, t_windowView.getSize().y * 0.11f);
	m_moneyText.setString("BALANCE: 400");
	m_moneyText.setFillColor(m_secondaryColor);
	m_moneyText.setOrigin(m_moneyText.getGlobalBounds().width / 2, 0.0f);

	// Setup shop item names
	m_shopItemNames.at(0).setString("Undo");
	m_shopItemNames.at(1).setString("Destroy\n  Tool");
	m_shopItemNames.at(2).setString("Redo");
	m_shopItemNames.at(3).setString("Bale");
	m_shopItemNames.at(4).setString("Mud");
	m_shopItemNames.at(5).setString("Treadmill");
	m_shopItemNames.at(6).setString("Stepping\n  Stone");
	m_shopItemNames.at(7).setString("  Mud\nTurret");
	m_shopItemNames.at(8).setString("Trapdoor");

	m_shopItemPrices.at(0).setString("$" + std::to_string(Global::getTilePrice(TileType::Wall))); // Wall
	m_shopItemPrices.at(1).setString("$" + std::to_string(Global::getTilePrice(TileType::Mud))); // Mud
	m_shopItemPrices.at(2).setString("$" + std::to_string(Global::getTilePrice(TileType::TreadmillWest))); // Treadmill
	m_shopItemPrices.at(3).setString("$" + std::to_string(Global::getTilePrice(TileType::SteppingStones))); // Balancing pad
	m_shopItemPrices.at(4).setString("$" + std::to_string(Global::getTilePrice(TileType::TurretWest))); // Turret
	m_shopItemPrices.at(5).setString("$" + std::to_string(Global::getTilePrice(TileType::Trapdoor))); // Trapdoor

	// t_windowView.getSize().y / 2.0f - 110.0f

	float spacing{ 32.0f };
	float baseX{ shopCentre - (m_shopItems.at(0).getSize().x * 1.5f) - spacing };

	// Setup the sheep button
	m_sheepButton.setPosition({ baseX + (m_sheepButton.getSize().x + spacing), m_VERTICAL_BUTTON_OFFSETS[0] - 112.0f });
	m_sheepButton.setup();

	m_sheepText.setString("Sheep");
	m_sheepText.setFont(m_hudFont);
	m_sheepText.setFillColor(sf::Color::Black);
	m_sheepText.setCharacterSize(13u);
	m_sheepText.setOrigin(m_sheepText.getGlobalBounds().width / 2.0f, 0.0f);
	m_sheepText.setPosition(m_sheepButton.getPosition().x, m_sheepButton.getPosition().y + m_sheepButton.getSize().y / 2.0f);

	m_sheepPrice.setString("$" + std::to_string(SHEEP_PRICE));
	m_sheepPrice.setFont(m_hudFont);
	m_sheepPrice.setFillColor(m_secondaryColor);
	m_sheepPrice.setCharacterSize(15u);
	m_sheepPrice.setOrigin(m_sheepPrice.getGlobalBounds().width / 2.0f, 0.0f);
	m_sheepPrice.setPosition(m_sheepText.getPosition().x, m_sheepText.getPosition().y + m_sheepText.getGlobalBounds().height + 5);


	// Setup the item names
	for (unsigned i = 0; i < m_shopItems.size(); i++)
	{
		m_shopItems.at(i).setPosition({ baseX + (m_shopItems.at(i).getSize().x + spacing) * (i % 3), m_VERTICAL_BUTTON_OFFSETS[static_cast<int>(i / 3)] });

		m_shopItems.at(i).setup();

		m_shopItemNames.at(i).setFont(m_hudFont);
		m_shopItemNames.at(i).setFillColor(sf::Color::Black);
		m_shopItemNames.at(i).setCharacterSize(13u);
		m_shopItemNames.at(i).setOrigin(m_shopItemNames.at(i).getGlobalBounds().width / 2.0f, 0.0f);
		m_shopItemNames.at(i).setPosition(m_shopItems.at(i).getPosition().x, m_shopItems.at(i).getPosition().y + m_shopItems.at(i).getSize().y / 2.0f);
	}

	// Setup the item prices
	for (unsigned i = 0; i < m_shopItemPrices.size(); i++)
	{
		m_shopItemPrices.at(i).setFont(m_hudFont);
		m_shopItemPrices.at(i).setFillColor(m_secondaryColor);
		m_shopItemPrices.at(i).setCharacterSize(15u);
		m_shopItemPrices.at(i).setOrigin(m_shopItemPrices.at(i).getGlobalBounds().width / 2.0f, 0.0f);
		m_shopItemPrices.at(i).setPosition(m_shopItemNames.at(i + 3).getPosition().x, m_shopItemNames.at(i + 3).getPosition().y + m_shopItemNames.at(i + 3).getGlobalBounds().height + 5);
	}

	// Setup the shop divider
	for (int i = 0; i < 2; i++)
	{
		m_shopDividers[i].setSize({ m_shopBackground.getSize().x * 0.8f, 2.0f });
		m_shopDividers[i].setFillColor(m_secondaryColor);
		m_shopDividers[i].setOrigin(m_shopDividers[i].getSize().x / 2.0f, 0.0f);
	}

	m_shopDividers[0].setPosition(shopCentre, m_sheepButton.getPosition().y + 68.0f);
	m_shopDividers[1].setPosition(shopCentre, m_VERTICAL_BUTTON_OFFSETS[0] + m_shopItems.at(0).getSize().y + 42.0f);

	// Setup the three game buttons
	m_playButton.setup();
	m_stopButton.setup();
	m_pauseButton.setup();
}

/////////////////////////////////////////////////////////////////
void HUD::setupStatsMenu(sf::View const& t_windowView)
{
	// Stats panel
	m_statsBackground.setSize({ t_windowView.getSize().x / 5.0f, t_windowView.getSize().y });
	m_statsBackground.setFillColor(m_mainColor);
	m_statsBackground.setPosition(t_windowView.getSize().x - m_statsBackground.getSize().x, 0.0f);

	m_statsBackground.setOutlineColor(m_secondaryColor);
	m_statsBackground.setOutlineThickness(5.0f);

	m_iconsPosition = { m_statsBackground.getPosition().x + m_statsBackground.getSize().x * 0.2f, t_windowView.getSize().y * 0.4f };

	m_iconSprite.setTexture(m_iconTextures);

	m_numAIText.setFont(m_hudFont);
	m_moneyEarnedText.setFont(m_hudFont);
	m_timeText.setFont(m_hudFont);

	m_numAIText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y);
	m_timeText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y + 32.0f * 1.2f);
	m_moneyEarnedText.setPosition(m_iconsPosition.x + 50.0f, m_iconsPosition.y + 32.0f * 1.2f * 2.0f);

	m_numAIText.setFillColor(m_secondaryColor);
	m_moneyEarnedText.setFillColor(m_secondaryColor);
	m_timeText.setFillColor(m_secondaryColor);

	m_numAIText.setCharacterSize(20u);
	m_moneyEarnedText.setCharacterSize(20u);
	m_timeText.setCharacterSize(20u);
}

/////////////////////////////////////////////////////////////////