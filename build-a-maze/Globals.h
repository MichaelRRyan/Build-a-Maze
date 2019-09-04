/// @Author Michael Rainsford Ryan

#ifndef GLOBALS_H
#define GLOBALS_H

const int MAZE_ROWS{ 15 };
const int MAZE_COLS{ 15 };
const float TILE_SIZE{ 32.0f };

const int BASIC_SOLVERS_MAX{ 6 };

const unsigned WINDOW_WIDTH{ 1280u };
const unsigned WINDOW_HEIGHT{ 720u };

const sf::IntRect WALL_TEXT_RECT{ 0, 832, 32, 64 };
const sf::IntRect PLANT_TEXT_RECT{ 480, 768, 32, 64 };

const float GAMEPLAY_SECTION_END{ 800.0f };
const float SIM_PANEL_START{ 1000 };

enum class ConstructionMode
{
	None,
	Placing,
	Destroying
};

enum TileType {
	None,
	Slow = 1,
	Wall = 10
};

enum class GameState {
	None,
	TitleScreen,
	SettingsScreen,
	BuildMode,
	Simulation
};

#endif // !GLOBALS_H

