#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "Globals.h"
#include "MazeSolver.h"
#include "Sheep.h"

class Renderer
{
public:
	Renderer(sf::RenderWindow& t_window, sf::View const & t_view, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_mazeRef, std::vector<MazeSolver*> const& t_solvers, std::vector<Sheep *> const& t_sheep);

	void drawMaze(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeWithSolvers(sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeBackground();

	void drawTileSelector(sf::Vector2i t_selectedTile);

	void drawMazeFloorRow(int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeWallRow(int t_row, sf::Vector2i t_selectedTile, ConstructionMode t_constructionMode, TileType t_selectedTileType);

	void drawMazeUI(sf::Vector2i t_selectedTile, TileType t_selectedTileType);

	void drawMazeHighlights();

	void drawTile(TileType t_tileType, int t_frame, int t_row, int t_col, sf::Color t_colorOverlay);

	void drawDirectionTile(Direction t_direction, int t_row, int t_col, sf::Color t_colorOverlay);

	void drawMazeSolvers(int t_row);

	void drawSheep(int t_row);

private:

	void setup();

	sf::Texture m_tileTexture;
	sf::Sprite m_textureTile;
	sf::Sprite m_caveSprite;
	sf::Clock m_animationClock;
	sf::RectangleShape m_tileSelector;
	sf::RectangleShape m_highlighter;

	// References
	sf::RenderWindow& m_windowRef;
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & m_mazeRef;
	std::vector<MazeSolver*> const& m_solversRef;
	std::vector<Sheep *> const& m_sheepRef;
	sf::View const& m_view;
};

#endif // !RENDERER_H