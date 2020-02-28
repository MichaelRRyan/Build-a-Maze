#ifndef SHEEP_H
#define SHEEP_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 27/02/2020
/// </summary>

#include <array>
#include "Globals.h"
#include "Tile.h"

class MazeSolver;

class Sheep
{
public:

	Sheep(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& t_maze);

	void update();
	void draw(sf::RenderWindow& t_window) const;

	void loadFiles();
	void reset();
	
	void setFollowing(MazeSolver* t_followee);

	void setPos(int t_row, int t_col);

	void setTimeModifier(float t_mod); // Set the time modifier for the movement speed

	const sf::Vector2i getPos() const; // Return the row and col position of the sheep
	const bool getActive() const;
	const sf::Sprite getSprite() const;
	const bool isFollowing() const; // Whether or not the sheep is following a solver

private:

	void animate();
	void setTextureDirection();
	bool isBlocked(sf::Vector2i t_mazePos);
	void updateWander();
	void handleTreadmills();

	enum class State
	{
		Idle,
		Wander,
		Follow
	};

	// Declare private data members
	const int IDLE_SPEED = 100;
	const int DEFAULT_MOVE_SPEED = 40;
	const int SLOW_MOVE_SPEED = 80;

	sf::Texture m_spriteSheet;
	sf::Sprite m_sprite;
	sf::Vector2i m_pos;
	sf::Vector2i m_previousPos; // Used for animation
	Direction m_moveDir;
	int m_moveTimer;
	int m_movementSpeed;
	float m_timeModifier;
	bool m_active;

	// Texture variables
	int m_characterDirection;

	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE>& m_mazeRef;

	State m_state;

	MazeSolver * m_followee;
};

#include "MazeSolver.h"

#endif // !SHEEP_H