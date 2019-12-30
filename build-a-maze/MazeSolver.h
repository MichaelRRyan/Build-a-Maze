#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

/// @Author Michael Rainsford Ryan
/// @Date 30/10/2019

#include <SFML/Graphics.hpp>
#include "Globals.h"

class MazeSolver
{
public:
	MazeSolver() = default;
	virtual void loadFiles() = 0;
	virtual void update(TileType t_maze[][MAZE_COLS]) = 0; // Move the enemy if not blocked by an enemy or wall
	virtual void move(TileType t_maze[][MAZE_COLS], sf::Vector2i t_newPosition); // Move the solver to a new position
	virtual void findNewDirection(TileType t_maze[][MAZE_COLS]); // Finds a new direction (direction solver is not facing). Always goes right or left before turning around
	virtual void reset(int t_moveDelay);
	virtual void draw(sf::RenderWindow& t_window) const;

	inline sf::Sprite getBody() { return m_body; }
	inline sf::Vector2i getPos() { return m_pos; } // Return the row and col position of the ghost
	inline bool getActive() { return m_active; };

	void setPos(int t_row, int t_col);
	inline void setMoveTimer(int t_moveDelay) { m_moveTimer = t_moveDelay; } // Set the delay between movements
	inline void setActive(bool t_active) { m_active = t_active; }
	inline void setCharacterDirection(int t_dir) { m_characterDirection = t_dir; }
	inline void setTimeModifier(float t_mod) { m_timeModifier = t_mod; } // Set the time modifier for the movement speed

	
	void checkForExit(TileType t_maze[][MAZE_COLS]); // Check if the exit of the maze is within sight, set direction towards it
	void animate();
	void setTextureDirection(); // Set the correct texture for the direction the enemy is facing

protected:
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

	// Texture variables
	sf::Vector2i m_characterNumber;
	int m_characterDirection;

	int tempTestVar;
};

#endif // !MAZE_SOLVER_H