#include "Paintball.h"

Paintball::Paintball() :
	m_active{ false }
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/tile_sheet.png"))
	{
		throw("Error loading paintball texture");
	}

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect({ 80, 64, 8, 8 });
	m_sprite.setOrigin(4.0f, 4.0f);
	m_sprite.setScale(2.0f, 2.0f);
}

void Paintball::fire(sf::Vector2f t_position, Direction t_direction, sf::Color t_color)
{
	m_active = true;

	m_sprite.setPosition(t_position);
	m_sprite.setColor(t_color);

	switch (t_direction)
	{
	case Direction::West:
		m_sprite.setRotation(-180.0f);
		break;
	case Direction::East:
		m_sprite.setRotation(0.0f);
		break;
	default:
		m_active = false;
		break;
	}

	m_velocity = static_cast<sf::Vector2f>(Global::getDirectionVector(t_direction)) * s_SPEED;
}

void Paintball::update(std::vector<MazeSolver*> t_solvers)
{
	if (m_active)
	{
		m_sprite.move(m_velocity);

		// Check horisontal maze bounds
		if (m_sprite.getPosition().x + 4.0f < 0.0f || m_sprite.getPosition().x - 4.0f > static_cast<float>(MAZE_SIZE)* TILE_SIZE)
		{
			m_active = false;
		}

		for (MazeSolver * solver : t_solvers)
		{
			if (solver->getActive())
			{
				if (solver->getSprite().getGlobalBounds().intersects(m_sprite.getGlobalBounds()))
				{
					solver->setActive(false);
					m_active = false;
					break;
				}
			}
		}
	}
}

const bool Paintball::isActive() const
{
	return m_active;
}

void Paintball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_active)
	{
		target.draw(m_sprite, states);
	}
}
