#include "Globals.h"

sf::Vector2i Global::getDirectionVector(Direction t_direction)
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

Direction Global::getDirection(sf::Vector2i t_directionVec)
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

Direction Global::getDirection(TileType t_tile)
{
	switch (t_tile)
	{
	case TileType::TreadmillWest:
		return Direction::West;
		break;
	case TileType::TreadmillEast:
		return Direction::East;
		break;
	case TileType::TreadmillNorth:
		return Direction::North;
		break;
	case TileType::TreadmillSouth:
		return Direction::South;
		break;
	case TileType::TurretWest:
		return Direction::West;
		break;
	case TileType::TurretEast:
		return Direction::East;
		break;
	default:
		return Direction::None;
		break;
	}
}

int Global::getTilePrice(TileType t_tileType)
{
	switch (t_tileType)
	{
	case TileType::None:
		return 0;
		break;
	case TileType::Mud:
		return 10;
		break;
	case TileType::TreadmillWest:
		return 20;
		break;
	case TileType::TreadmillEast:
		return 20;
		break;
	case TileType::TreadmillNorth:
		return 20;
		break;
	case TileType::TreadmillSouth:
		return 20;
		break;
	case TileType::SteppingStones:
		return 80;
		break;
	case TileType::TurretWest:
		return 200;
		break;
	case TileType::TurretEast:
		return 200;
		break;
	case TileType::Trapdoor:
		return 100;
		break;
	case TileType::Wall:
		return 20;
		break;
	default:
		return 0;
		break;
	}
}

// Should probably use a vector, this works for the sake of time
int Global::random(std::initializer_list<int> t_params)
{
	int index{ rand() % static_cast<int>(t_params.size()) };

	int i = 0;
	for (int value : t_params)
	{
		if (i == index)
		{
			return value;
		}

		i++;
	}

	return -1;
}
