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

	void setAnimFrameTime(float t_frameTime);

	void setStartFrame(int t_frame);

	void setMaxFrames(int t_maxFrames);

	// Operators
	bool operator==(TileType t_rhs) const;
	bool operator!=(TileType t_rhs) const;

private:

	TileType m_type;

	sf::Clock m_animationClock;
	float m_animFrameTime;

	int m_startFrame;
	int m_maxFrames;
	int m_frame;

	bool m_animating;
	bool m_loop;
};

#endif // !TILE_H