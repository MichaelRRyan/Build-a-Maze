/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include "BasicSolver.h"
#include "Globals.h"
#include "GUI.h"

enum class GameState {
	None,
	TitleScreen,
	BuildMode,
	Simulation,
	Pause
};

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

	void setupGame();
	void setupShapes();
	void generateMaze();

	sf::RenderWindow m_window;
	bool m_exitGame;

	sf::RectangleShape m_tileSelector;
	sf::Vector2i m_selectedTile;
	sf::Vector2i m_mousePosition;

	sf::Texture m_tileTextures;
	sf::Sprite m_textureBlock;

	int m_mazeBlocks[MAZE_ROWS][MAZE_COLS];

	BasicSolver m_basicSolvers[BASIC_SOLVERS_MAX];

	int m_currency;

	sf::Font m_mainFont;
	sf::Text m_aiInfoText;

	const sf::View m_gameplayView{ { 420.0f, 240.0f }, { static_cast<float>(WINDOW_WIDTH) * 0.75f, static_cast<float>(WINDOW_HEIGHT) * 0.75f} };

	GUI m_gui;
	GameState m_gamestate;

	ConstructionMode m_constructionState;
	TileType m_selectedTileType;

	float m_timeModifier;
	float m_prevTimeToComplete;
	float m_timeToComplete;
	int m_moneyEarned;
};

#endif // !GAME

