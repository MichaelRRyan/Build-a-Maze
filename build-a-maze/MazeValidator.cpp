#include "MazeValidator.h"

bool MazeValidator::isMazeSolvable(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze)
{
#ifdef MAZE_VALIDATOR_DEBUG
	sf::RenderWindow window{ sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32u }, "Build-a-Maze!" };
	std::vector<MazeSolver *> m_mazeSolvers;
	Renderer renderer(window, window.getDefaultView(), t_maze, m_mazeSolvers);
	sf::RectangleShape renderShape{ {TILE_SIZE, TILE_SIZE} };
#endif // MAZE_VALIDATOR_DEBUG

	std::array<std::array<bool, MAZE_SIZE>, MAZE_SIZE> tracedTiles{ false };

	std::stack<sf::Vector2i> movementHistory;

	// Start the maze at the top left corner
	int row = 1;
	int col = 1;

	bool deadEnd = false;

	// Set complete to false and start looping
	bool complete = false;
	bool solvable = false;

	while (!complete) {

		// If the maze hits a dead end
		if (deadEnd)
		{
			if (!movementHistory.empty())
			{
				movementHistory.pop();
			}

			if (movementHistory.empty()) // if empty
			{
				complete = true;
				break;
			}

			row = movementHistory.top().x;
			col = movementHistory.top().y;

			deadEnd = false;
		}
		else
		{
			// Add the current position to the stack
			movementHistory.push(sf::Vector2i{ row, col });

			// Set the current tile to a ground tile
			tracedTiles[row][col] = true;
		}

		// Dead end is true until a path has been found
		deadEnd = true;

		for (int i = 0; i < 4; i++)
		{
			// Check for exit
			if (row == MAZE_SIZE - 2 && col == MAZE_SIZE - 2)
			{
				// Maze is solvable
				complete = true;
				solvable = true;
				break;
			}

			switch (i) {
			case 0: // Up
				if (row - 1 >= 0 && t_maze[row - 1][col] != TileType::Wall
					&& t_maze[row - 1][col] != TileType::TurretWest
					&& !tracedTiles[row - 1][col]) {
					row -= 1;
					deadEnd = false;
				}
				break;
			case 1: // Down
				if (row + 1 < MAZE_SIZE && t_maze[row + 1][col] != TileType::Wall
					&& t_maze[row + 1][col] != TileType::TurretWest
					&& !tracedTiles[row + 1][col]) {
					row += 1;
					deadEnd = false;
				}
				break;
			case 2: // Left
				if (col - 1 >= 0 && t_maze[row][col - 1] != TileType::Wall
					&& t_maze[row][col - 1] != TileType::TurretWest
					&& !tracedTiles[row][col - 1]) {
					col -= 1;
					deadEnd = false;
				}
				break;
			case 3: // Right
				if (col + 1 < MAZE_SIZE && t_maze[row][col + 1] != TileType::Wall
					&& t_maze[row][col + 1] != TileType::TurretWest
					&& !tracedTiles[row][col + 1]) {
					col += 1;
					deadEnd = false;
				}
				break;
			}

			if (deadEnd == false)
			{
				break;
			}
		}

#ifdef MAZE_VALIDATOR_DEBUG
		window.clear();
		renderer.drawMaze({ 0, 0 }, ConstructionMode::None, TileType::None);

		renderShape.setFillColor(sf::Color{ 255, 0, 0, 100 });
		for (int i = 0; i < MAZE_SIZE; i++)
		{
			for (int j = 0; j < MAZE_SIZE; j++)
			{
				if (tracedTiles[i][j])
				{
					renderShape.setPosition(j * TILE_SIZE, i * TILE_SIZE);
					window.draw(renderShape);
				}
			}
		}

		renderShape.setFillColor(sf::Color{ 255, 0, 255, 100 });
		renderShape.setPosition(col * TILE_SIZE, row * TILE_SIZE);
		window.draw(renderShape);

		window.display();
		
		bool breakOut = false;;
		while (window.isOpen())
		{
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (sf::Event::Closed == event.type)
				{
					window.close();
				}
				if (sf::Event::KeyPressed == event.type)
				{
					if (sf::Keyboard::Space == event.key.code)
					{
						breakOut = true;
					}
				}
			}

			if (breakOut)
			{
				break;
			}
		}
#endif // MAZE_VALIDATOR_DEBUG
	}

	return solvable;
}
