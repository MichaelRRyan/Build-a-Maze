/// @Author Michael Rainsford Ryan
#ifndef BASIC_SOLVER_H
#define BASIC_SOLVER_H

#include <SFML\Graphics.hpp>
#include "Globals.h"
#include <iostream>

enum class Direction
{
	None,
	North,
	South,
	West,
	East
};

class BasicSolver
{
	// Declare private data members
	const int DEFAULT_MOVE_SPEED = 15;
	const int SLOW_MOVE_SPEED = 30;

	sf::Texture m_spriteSheet;
	sf::Sprite m_body;
	sf::Vector2i m_pos;
	sf::Vector2i m_previousPos; // Used for animation
	Direction m_moveDir;
	int m_moveTimer;
	int m_movementSpeed;
	bool m_active;

	// Texture variables
	sf::Vector2i m_characterNumber;
	int m_characterDirection;

public:
	BasicSolver();
	void loadFiles();

	inline sf::Sprite getBody() { return m_body; }
	inline sf::Vector2i getPos() { return m_pos; } // Return the row and col position of the ghost
	inline bool getActive() { return m_active; };

	inline void setMoveTimer(int t_moveDelay) { m_moveTimer = t_moveDelay; } // Set the delay between movements
	void setPos(int t_row, int t_col);

	void move(int t_maze[][MAZE_COLS]); // Move the enemy if not blocked by an enemy or wall
	void setTextureDirection(); // Set the correct texture for the direction the enemy is facing

	static sf::Vector2i getDirectionVector(Direction t_direction);
	static Direction getDirection(sf::Vector2i t_directionVec);
};

#endif // !BASIC_SOLVER_H