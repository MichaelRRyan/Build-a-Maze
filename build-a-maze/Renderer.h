#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <array>
#include "Globals.h"
#include "MazeSolver.h"

class Renderer
{
public:
	Renderer(sf::RenderWindow& t_window, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_mazeRef, std::vector<MazeSolver*> const& t_solvers);

	void setup();

	void drawMaze(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeWithSolvers(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeBackground();

	void drawTileSelector(sf::Vector2i t_selectedTile);

	void drawMazeRow( int row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeSolvers(int t_row);

private:
	sf::Texture m_tileTexture;
	sf::Sprite m_textureTile;
	sf::Clock m_animationClock;
	sf::RectangleShape m_tileSelector;

	// References
	sf::RenderWindow& m_windowRef;
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & m_mazeRef;
	std::vector<MazeSolver*> const& m_solversRef;
};

#endif // !RENDERER_H