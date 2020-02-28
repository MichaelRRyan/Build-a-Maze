#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

/// @Author Michael Rainsford Ryan
/// @Date 30/10/2019

#include <SFML/Graphics.hpp>
#include <array>
#include "Globals.h"
#include "Tile.h"
#include "Sheep.h"

class MazeSolver
{
public:
	MazeSolver(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze, std::vector<Sheep*>& t_sheepRef);
	virtual void loadFiles() = 0;
	virtual void update() = 0; // Move the enemy if not blocked by an enemy or wall
	virtual void move(sf::Vector2i t_newPosition); // Move the solver to a new position
	virtual void findNewDirection(); // Finds a new direction (direction solver is not facing). Always goes right or left before turning around
	virtual bool isBlocked(sf::Vector2i t_mazePos); // Check if a tile is a wall
	virtual void reset(int t_moveDelay);
	virtual void draw(sf::RenderWindow& t_window) const;

	inline sf::Vector2i getPreviousPos() { return m_previousPos; } // Return the previous row and col position of the solver
	inline sf::Vector2i getPos() { return m_pos; } // Return the row and col position of the solver
	inline bool getActive() { return m_active; };
	inline sf::Sprite getSprite() { return m_body; }

	inline void setActive(bool t_active) { m_active = t_active; }
	void setPos(int t_row, int t_col);
	inline void setTimeModifier(float t_mod) { m_timeModifier = t_mod; } // Set the time modifier for the movement speed

	void checkForExit(); // Check if the exit of the maze is within sight, set direction towards it
	void animate();
	void setTextureDirection(); // Set the correct texture for the direction the enemy is facing

	const int getMovementSpeed() const;

	virtual void hasFollower(bool t_hasFollower);

protected:

	void handleTreadmills();
	void handleSteppingStones();
	void handleTrapdoors();

	void checkForSheep(); // Check if there are any unclaimed sheep in sight
	void checkSheepCollisions();
	bool sheepAvailable();

	void checkIfOutOfMaze();

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

	std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & m_mazeRef;
	std::vector<Sheep *> & m_sheepRef;

	bool m_hasFollower; // Whether or not a sheep is following the solver
};

#endif // !MAZE_SOLVER_H