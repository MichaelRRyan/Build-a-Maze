/// @Author Michael Rainsford Ryan

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/Graphics.hpp>

const int MAZE_SIZE{ 15 };
const float TILE_SIZE{ 32.0f };

const sf::Vector2i MAZE_EXIT{ 0, 1 };
const sf::Vector2i GAME_EXIT{ -10, 8 };

const unsigned WINDOW_WIDTH{ 1280u };
const unsigned WINDOW_HEIGHT{ 720u };

const sf::IntRect WALL_TEXT_RECT{ 0, 80, 16, 32 };
const sf::IntRect MUD_TEXT_RECT{ 48, 64, 16, 16 };

const float GAMEPLAY_SECTION_END{ 800.0f };
const float SIM_PANEL_START{ 1000 };

const int SHEEP_PRICE{ 800 };

enum class ConstructionMode
{
	None,
	Placing,
	Destroying
};

enum class TileType {
	None,
	Mud = 1,
	TreadmillWest = 2,
	TreadmillEast = 3,
	TreadmillNorth = 4,
	TreadmillSouth = 5,
	SteppingStones = 6,
	TurretWest = 7,
	TurretEast = 8,
	Trapdoor = 9,
	Wall = 10
};

enum class GameState {
	None,
	TitleScreen,
	SettingsScreen,
	BuildMode,
	Simulation,
	GameEnd
};

enum class Direction
{
	None,
	North,
	South,
	West,
	East
};

class Global
{
public:
	static sf::Vector2i getDirectionVector(Direction t_direction);

	static Direction getDirection(sf::Vector2i t_directionVec);

	static Direction getDirection(TileType t_tile);

	static int getTilePrice(TileType t_tileType);

	static int random(std::initializer_list<int> t_params);
};

#endif // !GLOBALS_H

