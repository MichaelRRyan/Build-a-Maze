/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include "BasicSolver.h"
#include "Globals.h"
#include "GUI.h"

class Game
{
public:

	Game();
	~Game();

	void run();

private:
	// Private functions
	// Main functions
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	// Specific functions
	void setupGame();
	void setupShapes();
	void generateMaze();

	// Render window and views
	sf::RenderWindow m_window;
	const sf::View m_gameplayView;

	// Bools
	bool m_exitGame;
	bool m_simDetailsDisplay;
	bool m_gamePaused;

	// Numeric variables
	float m_timeModifier;
	float m_prevTimeToComplete;
	float m_timeToComplete;
	int m_currency;
	int m_moneyEarned;
	int m_noOfAI;

	// Maze array
	int m_mazeBlocks[MAZE_ROWS][MAZE_COLS];

	// Vectors
	sf::Vector2i m_selectedTile;
	sf::Vector2i m_mousePosition;

	// Shapes
	sf::RectangleShape m_tileSelector;
	sf::RectangleShape m_pauseScreenFade; // Shape to give the pause screen a fade effect

	// Sprites and textures
	sf::Texture m_tileTextures;
	sf::Sprite m_textureBlock;

	// Font and text
	sf::Font m_mainFont;
	sf::Text m_pauseText;

	// Objects
	BasicSolver m_basicSolvers[BASIC_SOLVERS_MAX];

	// Enum variables
	GUI m_gui;
	GameState m_gamestate;
	ConstructionMode m_constructionState;
	TileType m_selectedTileType;
};

#endif // !GAME

