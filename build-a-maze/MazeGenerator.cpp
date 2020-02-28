#include "MazeGenerator.h"

void MazeGenerator::generateMaze(std::array<std::array<Tile, MAZE_SIZE>, MAZE_SIZE> & t_maze)
{
	// Set all the blocks to walls
	for (int row = 0; row < MAZE_SIZE; row++)
	{
		for (int col = 0; col < MAZE_SIZE; col++)
		{
			t_maze[row][col].setType(TileType::Wall);
		}
	}

	// Create a stack to store the history of maze movements
	std::stack <sf::Vector2i> movementHistory;
	
	// Start the maze at the top left corner
	int row = 1;
	int col = 1;

	// Create the direction checked bools
	bool leftChecked, rightChecked, upChecked, downChecked;

	bool deadEnd = false;

	// Set complete to false and start looping
	bool complete = false;
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
			t_maze[row][col].setType(TileType::None);
		}

		// Set all direction checks to false
		leftChecked = false;
		rightChecked = false;
		upChecked = false;
		downChecked = false;

		// Create a foundDir bool and start looping
		bool foundDir = false;
		while (!foundDir)
		{
			// If all directions have been checked, break out
			if (leftChecked && rightChecked && upChecked && downChecked) {
				deadEnd = true;
				break;
			}

			// Choose a random direction
			int dir = rand() % 4;

			// Check the direction has not already been checked, if it has, choose a new direction
			if (dir == 0 && upChecked) {
				continue;
			}
			if (dir == 1 && downChecked) {
				continue;
			}
			if (dir == 2 && leftChecked) {
				continue;
			}
			if (dir == 3 && rightChecked) {
				continue;
			}


			switch (dir) {
			case 0:
				// Up
				if (row - 2 >= 0 && t_maze[row - 2][col] != TileType::None) {
					t_maze[row-1][col].setType(TileType::None);
					row -= 2;
					foundDir = true;
				}
				upChecked = true;
				break;
			case 1:
				// Down
				if (row + 2 < MAZE_SIZE && t_maze[row + 2][col] != TileType::None) {
					t_maze[row+1][col].setType(TileType::None);
					row += 2;
					foundDir = true;
				}
				downChecked = true;
				break;
			case 2:
				// Left
				if (col - 2 >= 0 && t_maze[row][col - 2] != TileType::None) {
					t_maze[row][col-1].setType(TileType::None);
					col -= 2;
					foundDir = true;
				}
				leftChecked = true;
				break;
			case 3:
				// Right
				if (col + 2 < MAZE_SIZE && t_maze[row][col + 2] != TileType::None) {
					t_maze[row][col+1].setType(TileType::None);
					col += 2;
					foundDir = true;
				}
				rightChecked = true;
				break;
			}
		}
	}

	// Clear the maze entrance and finish
	t_maze[1][0].setType(TileType::None);
}
