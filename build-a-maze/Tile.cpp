#include "Tile.h"

Tile::Tile() :
	m_type{ TileType::None },
	m_animating{ false },
	m_frame{ 0 },
	m_maxFrames{ 1 },
	m_animFrameTime{ 0 }
{
}

void Tile::updateAnimation()
{
	if (m_animating)
	{
		if (m_animationClock.getElapsedTime().asSeconds() > m_animFrameTime)
		{
			m_animationClock.restart();
			m_frame++;

			if (m_frame >= m_startFrame + m_maxFrames)
			{
				m_frame = m_startFrame;

				if (!m_loop)
				{
					m_animating = false;
				}
			}
		}
	}
}

bool Tile::operator==(TileType t_rhs) const
{
	return m_type == t_rhs;
}

bool Tile::operator!=(TileType t_rhs) const
{
	return  m_type != t_rhs;
}

void Tile::setType(TileType t_type)
{
	m_type = t_type;

	m_animating = false;
	m_frame = 0;
	m_startFrame = 0;

	if (m_type == TileType::TreadmillEast
		|| m_type == TileType::TreadmillWest
		|| m_type == TileType::TreadmillNorth
		|| m_type == TileType::TreadmillSouth)
	{
		m_maxFrames = 3;
		m_animFrameTime = 0.1f;
		m_startFrame = 1;
		m_loop = true;
		m_animating = true;
	}
	else if (m_type == TileType::SteppingStones)
	{
		m_maxFrames = 9;
		m_animFrameTime = 0.1f;
		m_loop = false;
	}
	else if (m_type == TileType::TurretWest
		|| m_type == TileType::TurretEast)
	{
		m_maxFrames = 7;
		m_animFrameTime = 0.6f;
		m_loop = false;
	}
	else if (m_type == TileType::Trapdoor)
	{
		m_maxFrames = 8;
		m_animFrameTime = 0.05f;
		m_startFrame = 4;
		m_frame = 4;
		m_loop = false;
	}
	else
	{
		m_maxFrames = 1;
		m_loop = false;
	}
}

TileType Tile::getType() const
{
	return m_type;
}

int Tile::getFrame() const
{
	return m_frame;
}

void Tile::setAnimating(bool t_animating)
{
	if (m_type == TileType::TreadmillEast
		|| m_type == TileType::TreadmillWest
		|| m_type == TileType::TreadmillNorth
		|| m_type == TileType::TreadmillSouth)
	{
		if (t_animating)
			m_startFrame = 1;
		else
			m_startFrame = 0;
	}

	m_animating = t_animating;
	m_frame = m_startFrame;
	m_animationClock.restart();

	if (m_type == TileType::TurretWest
		|| m_type == TileType::TurretEast)
	{
		m_frame = 1;
	}
}

bool Tile::getAnimating() const
{
	return m_animating;
}

void Tile::setAnimFrameTime(float t_frameTime)
{
	m_animFrameTime = t_frameTime;
}

void Tile::setStartFrame(int t_frame)
{
	m_startFrame = t_frame;
}

void Tile::setMaxFrames(int t_maxFrames)
{
	m_maxFrames = t_maxFrames;
}
