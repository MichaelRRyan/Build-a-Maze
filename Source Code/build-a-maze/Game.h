/// @Author Michael Rainsford Ryan

#ifndef GAME
#define GAME

//#define PLACE_SHEEP_DEBUG

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <iostream>

#include "MazeGenerator.h"
#include "MazeValidator.h"
#include "MazeEditor.h"

#include "SolverAnimator.h"

#include "MazeSolver.h"
#include "BasicSolver.h"
#include "Mathematician.h"
#include "Cartographer.h"
#include "Farmer.h"

#include "Globals.h"
#include "Config.h"
#include "Cursor.h"
#include "Renderer.h"
#include "MenuScreen.h"
#include "HUD.h"
#include "EndGameUI.h"
#include "Tile.h"
#include "Popup.h"
#include "Paintball.h"
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
	void setupObjects();

	void resetSimulation();
	void generateNewSolvers();
	void updateSimulation(sf::Time t_deltaTime);

	void processTimeModifierEvents(sf::Event t_event);
	void speedUpTime();
	void slowDownTime();
	void applyTimeModifier();

	void switchGameState();
	void handleClickEvents();
	void togglePause();

	void endGame();
	void startAnimatingMaze();
	void animateMaze();

	void placeSheep();
	void placeSheep(Sheep * t_sheep);
	void resetSheep();
	void updateSheep();
	void purchaseSheep();
	int countInaccessibleSheep();

	void drawSimulation();

	void exitToMenu();
	void restartGame();


	// ************************* Variables *************************

	// Render window and views
	sf::RenderWindow m_window;
	const sf::View m_GUI_VIEW;
	sf::View m_mazeView;

	sf::Clock m_animationClock;

	// Bools
	bool m_exitGame;
	bool m_simDetailsDisplay;
	bool m_gamePaused;
	bool m_animatingHUD;

	// Numeric variables
	const float m_BUILD_MODE_OFFSET;
	const float m_SIM_MODE_OFFSET;

	float m_timeModifier;
	float m_prevTimeToComplete;
	float m_timeToComplete;
	int m_currency;
	int m_moneyEarned;
	int m_noOfAI;

	int m_aliveAI;

	float m_secondsSinceSheepCheck;

	// Round variables
	const int m_STARTING_SOLVERS;
	int m_roundNumber;

	// Maze array
	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> m_mazeBlocks;

	// Shapes
	sf::RectangleShape m_pauseScreenFade; // Shape to give the pause screen a fade effect

	// Font and text
	sf::Font m_mainFont;
	sf::Text m_pauseText;

	// Game Objects
	std::vector<MazeSolver *> m_mazeSolverPtrs;
	std::vector<Sheep *> m_sheep;
	std::array<Paintball, 30> m_paintballs;

	Cursor m_cursor;

	// Enum variables
	GameState m_gamestate;
	GameState m_previousState;

	// UI Objects
	MenuScreen m_menuScreen;
	HUD m_hud;
	EndGameUI m_endGameUI;
	GUI::Popup m_popup;

	// Functional Objects
	Renderer m_renderer;
	MazeEditor m_mazeEditor;
	MazeValidator m_mazeValidator;
	SolverAnimator m_solverAnimator;

	// Audio
	sf::SoundBuffer m_turretSoundBuffer;
	sf::Sound m_turretSound;

	sf::Music m_backgroundMusic;
};

#endif // !GAME

