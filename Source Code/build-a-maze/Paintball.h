#ifndef PAINTBALL_H
#define PAINTBALL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "MazeSolver.h"
#include "Globals.h"

class Paintball : public sf::Drawable
{
public:

	Paintball();

	void fire(sf::Vector2f t_position, Direction t_direction, sf::Color t_color);

	void update(std::vector<MazeSolver *> t_solvers);

	const bool isActive() const;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	static constexpr float s_SPEED{ 3.0f };

	float m_startHeight;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;
	bool m_active;

	// Audio
	sf::SoundBuffer m_bulletThudBuffer;
	sf::Sound m_bulletThud;
};

#endif // !PAINTBALL_H
