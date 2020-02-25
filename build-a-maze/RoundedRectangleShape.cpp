#include "RoundedRectangleShape.h"

namespace sf
{
	///////////////////////////////////////////////////////////////////////////
	sf::RoundedRectangleShape::RoundedRectangleShape(sf::Vector2f t_size, float t_cornerRadius, std::size_t t_cornerPointCount) :
		m_size{ t_size },
		m_cornerRadius{ t_cornerRadius },
		m_cornerPointCount{ t_cornerPointCount }
	{
		update();
	}

	///////////////////////////////////////////////////////////////////////////
	void sf::RoundedRectangleShape::setSize(const sf::Vector2f t_size)
	{
		m_size = t_size;
		update();
	}

	///////////////////////////////////////////////////////////////////////////
	sf::Vector2f sf::RoundedRectangleShape::getSize() const
	{
		return m_size;
	}

	///////////////////////////////////////////////////////////////////////////
	void sf::RoundedRectangleShape::setCornerRadius(const float t_radius)
	{
		m_cornerRadius = t_radius;
		update();
	}

	///////////////////////////////////////////////////////////////////////////
	float sf::RoundedRectangleShape::getCornerRadius() const
	{
		return m_cornerRadius;
	}

	///////////////////////////////////////////////////////////////////////////
	void sf::RoundedRectangleShape::setCornerPointCount(const std::size_t t_cornerPointCount)
	{
		m_cornerPointCount = t_cornerPointCount;
		update();
	}

	///////////////////////////////////////////////////////////////////////////
	std::size_t sf::RoundedRectangleShape::getPointCount() const
	{
		return m_cornerPointCount * 4;
	}

	///////////////////////////////////////////////////////////////////////////
	sf::Vector2f sf::RoundedRectangleShape::getPoint(std::size_t t_index) const
	{
		if (t_index >= m_cornerPointCount * 4)
			return sf::Vector2f(0, 0);

		float deltaAngle = 90.0f / (m_cornerPointCount - 1);
		sf::Vector2f center;
		unsigned int centerIndex = t_index / m_cornerPointCount;
		static const float pi = 3.141592654f;

		switch (centerIndex)
		{
		case 0: center.x = m_size.x - m_cornerRadius; center.y = m_cornerRadius; break;
		case 1: center.x = m_cornerRadius; center.y = m_cornerRadius; break;
		case 2: center.x = m_cornerRadius; center.y = m_size.y - m_cornerRadius; break;
		case 3: center.x = m_size.x - m_cornerRadius; center.y = m_size.y - m_cornerRadius; break;
		}

		return sf::Vector2f(m_cornerRadius * cos(deltaAngle * (t_index - centerIndex) * pi / 180) + center.x,
			-m_cornerRadius * sin(deltaAngle * (t_index - centerIndex) * pi / 180) + center.y);
	}

	///////////////////////////////////////////////////////////////////////////
}