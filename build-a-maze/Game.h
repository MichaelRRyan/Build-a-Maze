/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include "BasicSolver.h"
#include "Globals.h"

class Game
{
public:

	Game();
	~Game();

	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void setupShapes();
	void generateMaze();

	sf::RenderWindow m_window;
	bool m_exitGame;

	sf::RectangleShape m_tileSelector;
	sf::Vector2i m_selectedTile;

	sf::Texture m_tileTextures;
	sf::Sprite m_textureBlock;

	int m_mazeBlocks[MAZE_ROWS][MAZE_COLS];

	BasicSolver m_basicSolvers[BASIC_SOLVERS_MAX];

	int m_currency;

	sf::Font m_mainFont;
	sf::Text m_currencyText;

	const sf::View m_gameplayView{ { 240.0f, 240.0f }, { 600.0f, 600.0f } };
};

#endif // !GAME

