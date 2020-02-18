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

			if (m_frame >= m_maxFrames)
			{
				m_frame = 0.0f;

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

	if (m_type == TileType::TreadmillEast
		|| m_type == TileType::TreadmillWest
		|| m_type == TileType::TreadmillNorth
		|| m_type == TileType::TreadmillSouth)
	{
		m_maxFrames = 3;
		m_animFrameTime = 0.1f;
		m_loop = true;
	}
	else if (m_type == TileType::SteppingStones)
	{
		m_maxFrames = 9;
		m_animFrameTime = 0.1f;
		m_loop = false;
	}
	else if (m_type == TileType::Turret)
	{
		m_maxFrames = 9;
		m_animFrameTime = 0.1f;
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
	m_animating = t_animating;
	m_frame = 0;
	m_animationClock.restart();
}

bool Tile::getAnimating() const
{
	return m_animating;
}
