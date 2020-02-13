#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "MazeSolver.h"

class Renderer
{
public:
	void setup();

	void drawMaze(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeWithSolvers(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], std::vector<MazeSolver*> const& t_solvers, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeBackground(sf::RenderWindow& t_window);

	void drawTileSelector(sf::RenderWindow& t_window, sf::Vector2i t_selectedTile);

	void drawMazeRow(sf::RenderWindow& t_window, TileType t_maze[][MAZE_COLS], int row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeSolvers(sf::RenderWindow& t_window, std::vector<MazeSolver *> const & t_solvers, int t_row);

private:
	sf::Texture m_tileTexture;
	sf::Sprite m_textureTile;
	sf::Clock m_animationClock;
	sf::RectangleShape m_tileSelector;
};

#endif // !RENDERER_H