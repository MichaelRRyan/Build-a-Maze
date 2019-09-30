/// @Author Michael Rainsford Ryan
/// @Date 28/09/2019
#ifndef MATHEMATICIAN_H
#define MATHEMATICIAN_

#include <SFML\Graphics.hpp>
#include "Globals.h"
#include <iostream>

class Mathematician
{
	// Declare private data members
	const int DEFAULT_MOVE_SPEED = 20;
	const int SLOW_MOVE_SPEED = 40;

	sf::Texture m_spriteSheet;
	sf::Sprite m_body;
	sf::Vector2i m_pos;
	sf::Vector2i m_previousPos; // Used for animation
	Direction m_moveDir;
	int m_moveTimer;
	int m_movementSpeed;
	float m_timeModifier;
	bool m_active;
	bool m_takeLefts; // True take all left turns, false take all right turns

	// Texture variables
	sf::Vector2i m_characterNumber;
	int m_characterDirection;

public:
	Mathematician();
	void loadFiles();

	inline sf::Sprite getBody() { return m_body; }
	inline sf::Vector2i getPos() { return m_pos; } // Return the row and col position of the ghost
	inline bool getActive() { return m_active; };

	inline void setMoveTimer(int t_moveDelay) { m_moveTimer = t_moveDelay; } // Set the delay between movements
	void setPos(int t_row, int t_col);
	inline void setActive(bool t_active) { m_active = t_active; }
	inline void setCharacterDirection(int t_dir) { m_characterDirection = t_dir; }
	inline void setTimeModifier(float t_mod) { m_timeModifier = t_mod; } // Set the time modifier for the movement speed

	void move(int t_maze[][MAZE_COLS]); // Move the enemy if not blocked by an enemy or wall
	void checkForExit(int t_maze[][MAZE_COLS]); // Check if the exit of the maze is within sight, set direction towards it
	void animate();
	void setTextureDirection(); // Set the correct texture for the direction the enemy is facing

	static sf::Vector2i getDirectionVector(Direction t_direction);
	static Direction getDirection(sf::Vector2i t_directionVec);
};

#endif // !MATHEMATICIAN_H