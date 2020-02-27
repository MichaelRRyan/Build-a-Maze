/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>

#include "MazeGenerator.h"
#include "MazeValidator.h"
#include "BasicSolver.h"
#include "Mathematician.h"
#include "Globals.h"
#include "Cursor.h"
#include "MazeSolver.h"
#include "Cartographer.h"
#include "Renderer.h"
#include "MenuScreen.h"
#include "HUD.h"
#include "Tile.h"
#include "Popup.h"
#include "Paintball.h"
#include "MazeEditor.h"
#include "Sheep.h"

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
	void update(sf::Time t_deltaTime);
	void render();

	// Game functions
	void setupGame();
	void resetSimulation();
	void updateSimulation(sf::Time t_deltaTime);
	void processTimeModifierEvents(sf::Event t_event);
	void switchGameState();
	void handleClickEvents();
	void togglePause();
	void startAnimatingMaze();
	void animateMaze();

	void placeSheep();
	void resetSheep();
	void updateSheep();


	// ************************* Variables *************************

	// Render window and views
	sf::RenderWindow m_window;
	const sf::View m_GUI_VIEW;
	sf::View m_mazeView;

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
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> m_mazeBlocks;

	// Shapes
	sf::RectangleShape m_pauseScreenFade; // Shape to give the pause screen a fade effect

	// Font and text
	sf::Font m_mainFont;
	sf::Text m_pauseText;

	// Objects
	std::vector<MazeSolver *> m_mazeSolverPtrs;
	std::vector<Sheep *> m_sheep;

	Cursor m_cursor;

	// Enum variables
	GameState m_gamestate;

	MenuScreen m_menuScreen;
	HUD m_hud;

	Renderer m_renderer;

	/// <summary>
	/// A temporary vector of tiles for buying from the shop. Reset every time the tiles are placed or the purchase is canceled
	/// </summary>
	std::vector<sf::Vector2i> m_tempTiles;

	GUI::Popup m_popup;

	std::array<Paintball, 30> m_paintballs;

	MazeEditor m_mazeEditor;
	MazeValidator m_mazeValidator;

	sf::Clock m_animationClock;

	const float m_BUILD_MODE_OFFSET;
	const float m_SIM_MODE_OFFSET;

	bool m_animating;
};

#endif // !GAME

