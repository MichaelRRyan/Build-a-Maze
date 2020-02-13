#include "Renderer.h"

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

void Renderer::drawMaze(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground(t_window);

	drawTileSelector(t_window, t_selectedTile);

	// Draw the walls
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		drawMazeRow(t_window, t_maze, row, t_selectedTile, t_constructionMode, t_selectedTileType);
	}
}

void Renderer::drawMazeWithSolvers(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], std::vector<MazeSolver*> const& t_solvers, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	drawMazeBackground(t_window);

	drawTileSelector(t_window, t_selectedTile);

	// Draw the walls and AI
	for (int row = 0; row < MAZE_ROWS; row++)
	{
		drawMazeRow(t_window, t_maze, row, t_selectedTile, t_constructionMode, t_selectedTileType);
		drawMazeSolvers(t_window, t_solvers, row);
	}
}

void Renderer::drawMazeBackground(sf::RenderWindow& t_window)
{
	// Draw the maze background (Grass)
	for (int row = -2; row < MAZE_ROWS + 2; row++)
	{
		for (int col = -8; col < MAZE_COLS + 2; col++)
		{
			m_textureTile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

			m_textureTile.setTextureRect(sf::IntRect{ (abs(row + col) % 3) * 32, 128, 32, 32 });
			t_window.draw(m_textureTile);
		}
	}
}

void Renderer::drawTileSelector(sf::RenderWindow& t_window, sf::Vector2i t_selectedTile)
{
	if (t_selectedTile.y > 0 && t_selectedTile.y < MAZE_ROWS - 1 && t_selectedTile.x > 0 && t_selectedTile.x < MAZE_COLS - 1)
	{
		m_tileSelector.setPosition(t_selectedTile.x * TILE_SIZE, t_selectedTile.y * TILE_SIZE);
		t_window.draw(m_tileSelector);
	}
}

void Renderer::drawMazeRow(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType)
{
	for (int col = 0; col < MAZE_COLS; col++)
	{
		if (static_cast<TileType>(t_maze[t_row][col]) != TileType::None)
		{
			// Draw blocks red to show removing ability
			if (t_row == t_selectedTile.y && col == t_selectedTile.x
				&& t_row > 0 && t_row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
				&& t_constructionMode == ConstructionMode::Destroying)
			{
				m_textureTile.setColor(sf::Color{ 200,50,50,245 });
			}

			m_textureTile.setPosition(col * TILE_SIZE, t_row * TILE_SIZE - 32.0f);

			if (static_cast<TileType>(t_maze[t_row][col]) == TileType::Mud)
			{
				m_textureTile.setTextureRect(PLANT_TEXT_RECT);
			}
			else if (static_cast<TileType>(t_maze[t_row][col]) == TileType::Wall) {
				m_textureTile.setTextureRect(WALL_TEXT_RECT);
			}
			else if (static_cast<TileType>(t_maze[t_row][col]) == TileType::TreadmillWest) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 0, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(t_maze[t_row][col]) == TileType::TreadmillEast) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 32, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(t_maze[t_row][col]) == TileType::TreadmillNorth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 64, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}
			else if (static_cast<TileType>(t_maze[t_row][col]) == TileType::TreadmillSouth) {
				int left = 32 * (static_cast<int>(m_animationClock.getElapsedTime().asMilliseconds()) / 2 % 4);
				m_textureTile.setTextureRect({ 32 + left, 96, 32, 32 });
				m_textureTile.move(0.0f, 32.0f);
			}

			t_window.draw(m_textureTile);
			m_textureTile.setColor(sf::Color::White);
		}
		// Draw ghost blocks when placing a tile
		else if (t_row == t_selectedTile.y && col == t_selectedTile.x
			&& t_row > 0 && t_row < MAZE_ROWS - 1 && col > 0 && col < MAZE_COLS - 1
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
			t_window.draw(m_textureTile);
			m_textureTile.setColor(sf::Color::White);
		}
	}
}

void Renderer::drawMazeSolvers(sf::RenderWindow& t_window, std::vector<MazeSolver*> const& t_solvers, int t_row)
{
	for (MazeSolver* solver : t_solvers)
	{
		if (solver->getActive())
		{
			if (solver->getPos().y == t_row)
			{
				solver->draw(t_window);
			}
		}
	}
}
