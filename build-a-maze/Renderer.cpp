#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& t_windowRef, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_mazeRef, std::vector<MazeSolver*> const& t_solvers) :
	m_windowRef{ t_windowRef },
	m_mazeRef{ t_mazeRef },
	m_solversRef{ t_solvers }
{
}

void Renderer::setup()
{
	if (!m_tileTexture.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		throw("Error loading tile textures");
	}
	
	m_textureTile.setTexture(m_tileTexture);
	m_textureTile.setScale(2.0f, 2.0f);

	m_tileSelector.setSize({ 32, 32 });
	m_tileSelector.setFillColor(sf::Color::Transparent);
	m_tileSelector.setOutlineColor(sf::Color::White);
	m_tileSelector.setOutlineThickness(3.0f);
}

void Renderer::drawMaze(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

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
	}
}

void Renderer::drawMazeWithSolvers(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

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
	}
}

void Renderer::drawMazeBackground()
{
	// Draw the maze background (Grass)
	for (int row = -2; row < MAZE_SIZE + 2; row++)
	{
		for (int col = -8; col < MAZE_SIZE + 2; col++)
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
				&& m_mazeRef[t_row][col] != TileType::Turret)
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
				&& t_selectedTileType != TileType::Turret)
			{
				m_mazeRef[t_row][col].updateAnimation();
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
			|| m_mazeRef[t_row][col] == TileType::Turret)
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
			if (m_mazeRef[t_row][col] == TileType::Turret)
			{
				m_mazeRef[t_row][col].updateAnimation();
				drawTile(TileType::Turret, m_mazeRef[t_row][col].getFrame(), t_row, col, color);
			}

			// Draw a ghost turret if placing a turret on a wall
			else if (m_mazeRef[t_row][col] == TileType::Wall
				&& t_row == t_selectedTile.y && col == t_selectedTile.x
				&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
				&& t_selectedTileType == TileType::Turret)
			{
				drawTile(TileType::Turret, 0, t_row, col, sf::Color{ 50,100,200,180 });
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
				m_mazeRef[t_row][col].updateAnimation();
				drawTile(TileType::Wall, 0, t_row, col, sf::Color{ 50,100,200,180 });
			}
		}
	}
}

void Renderer::drawTile(TileType t_tileType, int t_frame, int t_row, int t_col, sf::Color t_colorOverlay)
{
	m_textureTile.setPosition(t_col * TILE_SIZE, t_row * TILE_SIZE);
	m_textureTile.setOrigin(0.0f, 0.0f);
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
	case TileType::Turret:
		m_textureTile.setTextureRect({ 16 * t_frame, 128, 16, 32 });
		m_textureTile.move(0.0f, -32.0f);
		break;
	case TileType::Wall:
		m_textureTile.setTextureRect(WALL_TEXT_RECT);
		m_textureTile.move(0.0f, -32.0f);
		break;
	}

	m_windowRef.draw(m_textureTile);
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
