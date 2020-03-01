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
	virtual bool isBlocked(sf::Vector2i t_mazePos) const; // Check if a tile is a wall
	virtual void reset(int t_moveDelay);
	virtual void draw(sf::RenderWindow& t_window) const;
	virtual void animate();

	// Getters
	const sf::Vector2i getPreviousPos() const; // Return the previous row and col position of the solver
	const sf::Vector2i getPos() const; // Return the row and col position of the solver
	const bool getActive() const;
	const sf::Sprite getSprite() const;
	const int getMoveTimer() const;
	const bool isAnimatingIn() const;
	const bool isAnimatingOut() const;

	// Setters
	void setAnimatingIn(bool t_state);
	void setAnimatingOut(bool t_state);
	void setActive(bool t_active);
	void setPos(int t_row, int t_col);
	void setPreviousPos(int t_row, int t_col);
	void setMovementSpeed(int t_speed);
	void setTimeModifier(float t_mod); // Set the time modifier for the movement speed
	void setTimeModifier(float t_mod, int m_movementSpeed);
	void setMovementDirection(Direction t_direction);

	void die();
	void playHmmSound();
	void playFootstep();
	void playTriumphSound();

	void resetMoveTimer();
	void decrementMoveTimer();

	void checkForExit(); // Check if the exit of the maze is within sight, set direction towards it
	void setTextureDirection(); // Set the correct texture for the direction the enemy is facing

	const int getMovementSpeed() const;

	virtual void hasFollower(bool t_hasFollower);

protected:

	void loadAudioFiles();

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
	Direction m_previousMoveDir;
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
	bool m_animatingIn; // Whether or not the solver is animating into the maze
	bool m_animatingOut; // Whether or not the solver is animating out of the maze

	// Audio
	sf::SoundBuffer m_deathSoundBuffer;
	sf::Sound m_deathSound;

	sf::SoundBuffer m_hmmSoundBuffer;
	sf::Sound m_hmmSound;

	// Footsteps
	sf::SoundBuffer m_stepLeftBuffer;
	sf::Sound m_stepLeft;

	sf::SoundBuffer m_stepRightBuffer;
	sf::Sound m_stepRight;

	bool m_leftStep; // Bool for footstep audio

	// Triump
	sf::SoundBuffer m_triumphSoundBuffer;
	sf::Sound m_triumphSound;

	// TEMP move to different class if possible
	sf::SoundBuffer m_trapdoorCloseBuffer;
	sf::Sound m_trapdoorClose;
};

#endif // !MAZE_SOLVER_H