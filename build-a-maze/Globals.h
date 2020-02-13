/// @Author Michael Rainsford Ryan

#ifndef GLOBALS_H
#define GLOBALS_H

const int MAZE_ROWS{ 15 };
const int MAZE_COLS{ 15 };
const float TILE_SIZE{ 32.0f };

const int SOLVERS_MAX{ 8 };

const unsigned WINDOW_WIDTH{ 1280u };
const unsigned WINDOW_HEIGHT{ 720u };

const sf::IntRect WALL_TEXT_RECT{ 64, 160, 32, 64 };
const sf::IntRect PLANT_TEXT_RECT{ 32, 160, 32, 64 };

const float GAMEPLAY_SECTION_END{ 800.0f };
const float SIM_PANEL_START{ 1000 };

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
	Wall = 10
};

enum class GameState {
	None,
	TitleScreen,
	SettingsScreen,
	BuildMode,
	Simulation
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
	static sf::Vector2i getDirectionVector(Direction t_direction)
	{
		switch (t_direction)
		{
		case Direction::None:
			return sf::Vector2i{ 0,0 }; // Return a null vector for a null direction
		case Direction::North:
			return sf::Vector2i{ 0,-1 }; // Return a north vector
		case Direction::South:
			return sf::Vector2i{ 0,1 };  // Return a south vector
		case Direction::West:
			return sf::Vector2i{ -1,0 };  // Return a west vector
		case Direction::East:
			return sf::Vector2i{ 1,0 };  // Return a east vector
		default:
			return sf::Vector2i{ 0,0 }; // Return a null vector for a null direction
		}
	}

	static Direction getDirection(sf::Vector2i t_directionVec)
	{
		if (t_directionVec == sf::Vector2i{ 0, -1 })
		{
			return Direction::North;
		}
		else if (t_directionVec == sf::Vector2i{ 0, 1 })
		{
			return Direction::South;
		}
		else if (t_directionVec == sf::Vector2i{ -1, 0 })
		{
			return Direction::West;
		}
		if (t_directionVec == sf::Vector2i{ 1, 0 })
		{
			return Direction::East;
		}
		else
		{
			return Direction::None;
		}
	}
};

#endif // !GLOBALS_H

