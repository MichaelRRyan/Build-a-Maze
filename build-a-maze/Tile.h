#ifndef TILE_H
#define TILE_H

/// @Author Michael Rainsford Ryan
/// @Date 18/02/2020

#include <SFML/Graphics.hpp>
#include "Globals.h"

class Tile
{
public:
	Tile();

	void updateAnimation();

	// Getters and Setters
	void setType(TileType t_type);
	TileType getType() const;
	int getFrame() const;

	void setAnimating(bool t_animating);
	bool getAnimating() const;

	// Operators
	bool operator==(TileType t_rhs) const;
	bool operator!=(TileType t_rhs) const;

private:
	TileType m_type;
	bool m_animating;
	int m_maxFrames;
	int m_frame;
	float m_animFrameTime;
	sf::Clock m_animationClock;
	bool m_loop;
};

#endif // !TILE_H