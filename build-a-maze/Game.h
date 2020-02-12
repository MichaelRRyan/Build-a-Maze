/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "MazeGenerator.h"
#include "BasicSolver.h"
#include "Mathematician.h"
#include "Globals.h"
#include "GUI.h"
#include "XBox360Controller.h"
#include "Cursor.h"
#include "MazeSolver.h"
#include "Cartographer.h"
#include "Renderer.h"
#include "MenuScreen.h"
#include "HUD.h"

class Game
{
public:
	// ********************** Public Functions **********************
	Game();
	~Game();

	void run();

private:
	// ********************* Private Functions *********************
	// Engine functions
	void processEvents();
	void processKeyboardEvents(sf::Event t_event);
	void processMouseEvents(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	// Game functions
	void setupGame();
	void resetSimulation();
	void processTimeModifierEvents(sf::Event t_event);
	void updateCursor();
	void updateController();


	// ************************* Variables *************************

	// Render window and views
	sf::RenderWindow m_window;
	const sf::View m_GUI_VIEW;
	sf::View m_mazeView;

	// Bools
	bool m_exitGame;
	bool m_simDetailsDisplay;
	bool m_gamePaused;
	bool m_controllerConnected;

	// Numeric variables
	float m_timeModifier;
	float m_prevTimeToComplete;
	float m_timeToComplete;
	int m_currency;
	int m_moneyEarned;
	int m_noOfAI;
	float m_controllerSensitivity;

	// Maze array
	// Should be moved to a std::array
	TileType m_mazeBlocks[MAZE_ROWS][MAZE_COLS];

	// Vectors
	sf::Vector2i m_selectedTile;

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
	std::vector<MazeSolver *> m_mazeSolverPtrs;

	//BasicSolver m_basicSolvers[BASIC_SOLVERS_MAX];
	//Mathematician m_mathematicians[BASIC_SOLVERS_MAX];
	XBox360Controller m_controller;
	Cursor m_cursor;

	// Enum variables
	Screens m_gui;
	GameState m_gamestate;
	ConstructionMode m_constructionState;
	TileType m_selectedTileType;

	sf::Clock m_treadmillAnimClock;

	MenuScreen m_menuScreen;
	HUD m_hud;
};

#endif // !GAME

