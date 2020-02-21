#ifndef CURSOR_H
#define CURSOR_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 14/10/2019
/// </summary>

#include <SFML/Graphics.hpp>
#include <array>
#include "XBox360Controller.h"
#include "Globals.h"
#include "Tile.h"

/// <summary>
/// @brief A struct to replace the cursor to use custom cursors and support controllers
/// </summary>
class Cursor : public sf::Drawable
{
public:

	Cursor();

	void update(sf::RenderWindow const& t_window, std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> const & m_maze, GameState t_gameState,
				ConstructionMode t_constructionState, sf::View const & t_guiView, sf::View const & t_mazeView);

	bool m_clicked;
	bool m_cancelClicked;

	sf::Vector2i m_position;
	sf::Vector2f m_viewPosition;
	sf::Vector2i m_selectedTile;

private:

	/// <summary>
	/// @brief Update the controller and take input
	/// </summary>
	void updateController();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	XBox360Controller m_controller;
	float m_controllerSensitivity;
	bool m_controllerConnected;
};

#endif // !CURSOR_H
