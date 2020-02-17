#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& t_windowRef, std::array<std::array<TileType, MAZE_SIZE>, MAZE_SIZE> const& t_mazeRef, std::vector<MazeSolver*> const& t_solvers) :
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

	m_tileSelector.setSize({ 32, 32 });
	m_tileSelector.setFillColor(sf::Color::Transparent);
	m_tileSelector.setOutlineColor(sf::Color::White);
	m_tileSelector.setOutlineThickness(3.0f);
}

void Renderer::drawMaze(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

	drawTileSelector(t_selectedTile);

	// Draw the walls
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		drawMazeRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
	}
}

void Renderer::drawMazeWithSolvers(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground();

	drawTileSelector(t_selectedTile);

	// Draw the walls and AI
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		drawMazeRow(row, t_selectedTile, t_constructionMode, t_selectedTileType);
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

			m_textureTile.setTextureRect(sf::IntRect{ (abs(row + col) % 3) * 32, 128, 32, 32 });
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

void Renderer::drawMazeRow(int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	for (int col = 0; col < MAZE_SIZE; col++)
	{
		if (static_cast<TileType>(m_mazeRef[t_row][col]) != TileType::None)
		{
			// Draw blocks red to show removing ability
			if (t_row == t_selectedTile.y && col == t_selectedTile.x
				&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
				&& t_constructionMode == ConstructionMode::Destroying)
			{
				m_textureTile.setColor(sf::Color{ 200,50,50,245 });
			}

			m_textureTile.setPosition(col * TILE_SIZE, t_row * TILE_SIZE - 32.0f);

			if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::Mud)
			{
				m_textureTile.setTextureRect(PLANT_TEXT_RECT);
			}
			else if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::Wall) {
				m_textureTile.setTextureRect(WALL_TEXT_RECT);
			}
			else if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::TreadmillWest) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 0, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::TreadmillEast) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 32, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::TreadmillNorth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 64, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(m_mazeRef[t_row][col]) == TileType::TreadmillSouth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 96, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}

			m_windowRef.draw(m_textureTile);
			m_textureTile.setColor(sf::Color::White);
		}
		// Draw ghost blocks when placing a tile
		else if (t_row == t_selectedTile.y && col == t_selectedTile.x
			&& t_row > 0 && t_row < MAZE_SIZE - 1 && col > 0 && col < MAZE_SIZE - 1
			&& t_constructionMode == ConstructionMode::Placing)
		{
			m_textureTile.setPosition(col * TILE_SIZE, t_row * TILE_SIZE - 32.0f);

			if (t_selectedTileType == TileType::Mud)
			{
				m_textureTile.setTextureRect(PLANT_TEXT_RECT);
			}
			else if (t_selectedTileType == TileType::Wall) {
				m_textureTile.setTextureRect(WALL_TEXT_RECT);
			}
			else if (t_selectedTileType == TileType::TreadmillWest) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 0, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (t_selectedTileType == TileType::TreadmillEast) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 32, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (t_selectedTileType == TileType::TreadmillNorth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 64, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (t_selectedTileType == TileType::TreadmillSouth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 96, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}

			m_textureTile.setColor(sf::Color{ 50,100,200,180 });
			m_windowRef.draw(m_textureTile);
			m_textureTile.setColor(sf::Color::White);
		}
	}
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
