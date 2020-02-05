#include "Renderer.h"

void Renderer::drawMazeBackground(sf::RenderWindow& t_window, sf::Sprite t_tileSprite)
{
	// Draw the maze background (Grass)
	for (int row = -2; row < MAZE_ROWS + 2; row++)
	{
		for (int col = -8; col < MAZE_COLS + 2; col++)
		{
			t_tileSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);

			t_tileSprite.setTextureRect(sf::IntRect{ (abs(row + col) % 3) * 32, 128, 32, 32 });
			t_window.draw(t_tileSprite);
		}
	}
}
