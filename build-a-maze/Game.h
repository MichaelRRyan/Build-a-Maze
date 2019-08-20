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

	sf::Texture m_tileTextures;
	sf::Sprite m_textureBlock;

	int m_mazeBlocks[MAZE_ROWS][MAZE_COLS];

	BasicSolver m_basicSolvers[BASIC_SOLVERS_MAX];
};

#endif // !GAME

