#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& t_windowRef, sf::View const& t_view, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_mazeRef, std::vector<MazeSolver*> const& t_solvers, std::vector<Sheep *> const& t_sheep) :
	m_windowRef{ t_windowRef },
	m_mazeRef{ t_mazeRef },
	m_solversRef{ t_solvers },
	m_sheepRef{ t_sheep },
	m_view{ t_view }
{
	setup();
}

void Renderer::setup()
{
	if (!m_tileTexture.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		throw("Error loading tile textures");
	}
	
	m_textureTile.setTexture(m_tileTexture);
	m_textureTile.setScale(2.0f, 2.0f);

	m_caveSprite.setTexture(m_tileTexture);
	m_caveSprite.setPosition((GAME_EXIT.x + 3) * TILE_SIZE, (GAME_EXIT.y - 2) * TILE_SIZE);
	m_caveSprite.setScale(2.0f, 2.0f);

	m_tileSelector.setSize({ 32, 32 });
	m_tileSelector.setFillColor(sf::Color::Transparent);
	m_tileSelector.setOutlineColor(sf::Color::White);
	m_tileSelector.setOutlineThickness(3.0f);

	m_highlighter.setOutlineThickness(5.0f);
	m_highlighter.setOutlineColor(sf::Color{ 255,255,255,100 });
	m_highlighter.setFillColor(sf::Color::Transparent);
}

void Renderer::drawMaze(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

	m_caveSprite.setTextureRect({ 0, 160, 32, 64 });
	m_windowRef.draw(m_caveSprite);

	drawTileSelector(t_selectedTile);

	// Draw the floor tiles
	for (int row = 1; row < MAZE_SIZE - 1; row++)
	{
		drawMazeFloorRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
	}

	// Draw the walls
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		drawMazeWallRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
		drawSheep(row);
	}

	// Draw maze UI
	drawMazeUI(t_selectedTile, t_selectedTileType);

	m_caveSprite.setTextureRect({ 32, 160, 32, 64 });
	m_windowRef.draw(m_caveSprite);
}

void Renderer::drawMazeWithSolvers(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

	m_caveSprite.setTextureRect({ 0, 160, 32, 64 });
	m_windowRef.draw(m_caveSprite);

	drawTileSelector(t_selectedTile);

	// Draw the floor tiles
	for (int row = 1; row < MAZE_SIZE - 1; row++)
	{
		drawMazeFloorRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
	}

	// Draw the walls and AI
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		drawMazeWallRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
		drawMazeSolvers(row);
		drawSheep(row);
	}

	m_caveSprite.setTextureRect({ 32, 160, 32, 64 });
	m_windowRef.draw(m_caveSprite);

	drawMazeHighlights();
}

void Renderer::drawMazeBackground()
{
	m_textureTile.setColor(sf::Color::White);

	sf::Vector2i startPoint = static_cast<sf::Vector2i>((-(m_view.getSize() / 2.0f) + m_view.getCenter()) / TILE_SIZE); // bottom right corner of screen divided by tile size
	sf::Vector2i endPoint = static_cast<sf::Vector2i>(((m_view.getSize() / 2.0f) + m_view.getCenter()) / TILE_SIZE); // bottom right corner of screen divided by tile size

	// Draw the maze background (Grass)
	for (int row = startPoint.y - 1; row <= endPoint.y; row++)
	{
		for (int col = startPoint.x - 1; col <= endPoint.x; col++)
		{
			m_textureTile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

			m_textureTile.setTextureRect(sf::IntRect{ (abs(row + col) % 3) * 16, 16 * 4, 16, 16 });
			m_windowRef.draw(m_textureTile);
		}
	}
}

void Renderer::drawTileSelector(sf::Vector2i t_selectedTile)
{
	if (t_selectedTile.y > 0 && t_selectedTile.y < MAZE_SIZE - 1 && t_selectedTile.x > 0 && t_selectedTile.x < MAZE_SIZE - 1)
	{
		m_tileSelector.setPosition(t_selectedTile.x * TILE_SIZE, t_selectedTile.y * TILE_SIZE);
		m_windowRef.draw(m_tileSelector);
	}
}

void Renderer::drawMazeFloorRow(int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	for (int col = 1; col < MAZE_SIZE - 1; col++)
	{
		if (m_mazeRef[t_row][col] != TileType::None)
		{
			if (m_mazeRef[t_row][col] != TileType::Wall
				&& m_mazeRef[t_row][col] != TileType::TurretWest
				&& m_mazeRef[t_row][col] != TileType::TurretEast)
			{
				sf::Color color{ sf::Color::White };

				// Draw blocks red to show removing ability
				if (t_row == t_selectedTile.y && col == t_selectedTile.x
					&& t_constructionMode == ConstructionMode::Destroying)
				{
					color = sf::Color{ 200,50,50,245 };
				}

				m_mazeRef[t_row][col].updateAnimation();
				drawTile(m_mazeRef[t_row][col].getType(), m_mazeRef[t_row][col].getFrame(), t_row, col, color);
			}
		}
		// Draw ghost blocks when placing a tile
		else if (t_row == t_selectedTile.y && col == t_selectedTile.x
			&& t_constructionMode == ConstructionMode::Placing)
		{
			if (t_selectedTileType != TileType::Wall
				&& t_selectedTileType != TileType::TurretWest)
			{
				drawTile(t_selectedTileType, 0, t_row, col, sf::Color{ 50,100,200,180 });
			}
		}
	}
}

void Renderer::drawMazeWallRow(int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	for (int col = 0; col < MAZE_SIZE; col++)
	{
		if (m_mazeRef[t_row][col] == TileType::Wall
			|| m_mazeRef[t_row][col] == TileType::TurretWest
			|| m_mazeRef[t_row][col] == TileType::TurretEast)
		{
			sf::Color color{ sf::Color::White };

			// Draw blocks red to show removing ability
			if (t_row == t_selectedTile.y && col == t_selectedTile.x
				&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
				&& t_constructionMode == ConstructionMode::Destroying)
			{
				color = sf::Color{ 200,50,50,245 };
			}

			// Always draw a wall
			drawTile(TileType::Wall, 0, t_row, col, color);

			// Draw a turret if a turret tile
			if (m_mazeRef[t_row][col] == TileType::TurretWest
				|| m_mazeRef[t_row][col] == TileType::TurretEast)
			{
				m_mazeRef[t_row][col].updateAnimation();
				drawTile(m_mazeRef[t_row][col].getType(), m_mazeRef[t_row][col].getFrame(), t_row, col, color);
			}

			// Draw a ghost turret if placing a turret on a wall
			else if (m_mazeRef[t_row][col] == TileType::Wall
				&& t_row == t_selectedTile.y && col == t_selectedTile.x
				&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
				&& (t_selectedTileType == TileType::TurretWest
					|| t_selectedTileType == TileType::TurretEast))
			{
				drawTile(t_selectedTileType, 0, t_row, col, sf::Color{ 50,100,200,180 });
			}

		}
		// Draw ghost blocks when placing a tile
		else if (m_mazeRef[t_row][col] == TileType::None
			&& t_row == t_selectedTile.y && col == t_selectedTile.x
			&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
			&& t_constructionMode == ConstructionMode::Placing)
		{
			if (t_selectedTileType == TileType::Wall)
			{
				drawTile(TileType::Wall, 0, t_row, col, sf::Color{ 50,100,200,180 });
			}
		}
	}
}

void Renderer::drawMazeUI(sf::Vector2i t_selectedTile, TileType t_selectedTileType)
{
	if (t_selectedTile.x > 0 && t_selectedTile.x < MAZE_SIZE - 1
		&& t_selectedTile.y > 0 && t_selectedTile.y < MAZE_SIZE - 1)
	{
		if (t_selectedTileType >= TileType::TreadmillWest
			&& t_selectedTileType <= TileType::TreadmillSouth
			&& m_mazeRef[t_selectedTile.y][t_selectedTile.x] == TileType::None)
		{
			drawDirectionTile(Global::getDirection(t_selectedTileType), t_selectedTile.y, t_selectedTile.x, sf::Color{ 255,255,255,180 });
		}
		else if ((t_selectedTileType == TileType::TurretWest || t_selectedTileType == TileType::TurretEast)
			&& m_mazeRef[t_selectedTile.y][t_selectedTile.x] == TileType::Wall)
		{
			drawDirectionTile(Global::getDirection(t_selectedTileType), t_selectedTile.y, t_selectedTile.x, sf::Color{ 255,255,255,180 });
		}
	}
}

void Renderer::drawMazeHighlights()
{
	// Loop through the maze
	for (int row = 0; row < MAZE_SIZE - 1; row++)
	{
		for (int col = 1; col < MAZE_SIZE - 1; col++)
		{
			if (m_mazeRef[row][col] == TileType::Trapdoor)
			{
				if (m_mazeRef[row][col].getFrame() == 0)
				{
					float animFrame = m_animationClock.getElapsedTime().asMilliseconds() / 100 % 6;

					if (animFrame > 3)
						animFrame = 6 - animFrame;

					m_highlighter.setSize({ TILE_SIZE + animFrame * 2.0f, TILE_SIZE + animFrame * 2.0f });
					m_highlighter.setPosition(col * TILE_SIZE - animFrame, row * TILE_SIZE - animFrame);

					m_windowRef.draw(m_highlighter);
				}
			}
			if (m_mazeRef[row + 1][col] == TileType::TurretWest
				|| m_mazeRef[row + 1][col] == TileType::TurretWest)
			{
				if (!m_mazeRef[row + 1][col].getAnimating())
				{
					float animFrame = m_animationClock.getElapsedTime().asMilliseconds() / 100 % 6;

					if (animFrame > 3)
						animFrame = 6 - animFrame;

					m_highlighter.setSize({ TILE_SIZE + animFrame * 2.0f, TILE_SIZE + animFrame * 2.0f });
					m_highlighter.setPosition(col * TILE_SIZE - animFrame, row * TILE_SIZE - animFrame);

					m_windowRef.draw(m_highlighter);
				}
			}
		}
	}
}

void Renderer::drawTile(TileType t_tileType, int t_frame, int t_row, int t_col, sf::Color t_colorOverlay)
{
	m_textureTile.setPosition(t_col * TILE_SIZE, t_row * TILE_SIZE);
	m_textureTile.setColor(t_colorOverlay);

	switch (t_tileType)
	{
	case TileType::Mud:
		m_textureTile.setTextureRect(MUD_TEXT_RECT);
		break;
	case TileType::TreadmillWest:
		m_textureTile.setTextureRect({ 16 * t_frame, 0, 16, 16 });
		break;
	case TileType::TreadmillEast:
		m_textureTile.setTextureRect({ 16 * t_frame, 16, 16, 16 });
		break;
	case TileType::TreadmillNorth:
		m_textureTile.setTextureRect({ 16 * t_frame, 32, 16, 16 });
		break;
	case TileType::TreadmillSouth:
		m_textureTile.setTextureRect({ 16 * t_frame, 48, 16, 16 });
		break;
	case TileType::SteppingStones:
		m_textureTile.setTextureRect({ 16 * t_frame, 112, 16, 16 });
		break;
	case TileType::TurretWest:
		m_textureTile.setTextureRect({ 16 * t_frame, 128, 16, 16 });
		m_textureTile.move(0.0f, -32.0f);
		break;
	case TileType::TurretEast:
		m_textureTile.setTextureRect({ 16 * t_frame, 144, 16, 16 });
		m_textureTile.move(0.0f, -32.0f);
		break;
	case TileType::Trapdoor:
		m_textureTile.setTextureRect({ 16 + 16 * t_frame, 96, 16, 16 });
		break;
	case TileType::Wall:
		m_textureTile.setTextureRect(WALL_TEXT_RECT);
		m_textureTile.move(0.0f, -32.0f);
		break;
	}

	m_windowRef.draw(m_textureTile);
}

void Renderer::drawDirectionTile(Direction t_direction, int t_row, int t_col, sf::Color t_colorOverlay)
{
	m_textureTile.setColor(t_colorOverlay);

	m_textureTile.setTextureRect({ 64, 64, 16, 16 });

	switch (t_direction)
	{
	case Direction::North:
		m_textureTile.setRotation(-90.0f);
		m_textureTile.setPosition((t_col) * TILE_SIZE, (t_row) * TILE_SIZE);
		break;
	case Direction::South:
		m_textureTile.setRotation(90.0f);
		m_textureTile.setPosition((t_col + 1) * TILE_SIZE, (t_row + 1) * TILE_SIZE);
		break;
	case Direction::West:
		m_textureTile.setRotation(180.0f);
		m_textureTile.setPosition((t_col) * TILE_SIZE, (t_row + 1) * TILE_SIZE);
		break;
	case Direction::East:
		m_textureTile.setRotation(0.0f);
		m_textureTile.setPosition((t_col + 1) * TILE_SIZE, (t_row) * TILE_SIZE);
		break;
	}

	m_windowRef.draw(m_textureTile);

	m_textureTile.setRotation(0.0f);
}

void Renderer::drawMazeSolvers(int t_row)
{
	for (MazeSolver* solver : m_solversRef)
	{
		if (solver->getActive())
		{
			if (solver->getPos().y == t_row)
			{
				solver->draw(m_windowRef);
			}
		}
	}
}

void Renderer::drawSheep(int t_row)
{
	for (Sheep const* sheep : m_sheepRef)
	{
		if (sheep->getActive())
		{
			if (sheep->getPos().y == t_row)
			{
				sheep->draw(m_windowRef);
			}
		}
	}
}
