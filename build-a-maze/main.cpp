/// Basic Game
/// @Author Michael Rainsford Ryan
/// @Date 19/08/2019
/// Work Sessions:
/// 19/08/2019: 16:34 - Set up basic game files and created project
/// 19/08/2019: 16:34 - 18:02 - created and set up a 2D array and rendered. Started on maze generation algorithm
/// 19/08/2019: 18:33 - 19:35 - Finished maze generation algorithm and added basic placehold textures
/// 20/08/2019: 11:14 - 11:52 - Added basic AI
/// 20/08/2019: 12:44 - 12:50 - Edited rendering order of the tiles and AI to add slight 3D overlap effect
/// 21/08/2019: 14:51 - 15:05 - Began editing AI to make them solve the maze a little better
/// 21/08/2019: 16:03 - 16:20
/// 21/08/2019: 17:55 - 18:02 - Finished editing AI. They now have a 50 chance of changing direction to their left or right whenever a pathway appears 
/// 21/08/2019: 23:41 - 22/08/2019: 00:31 - Added money, placing and removing walls, and resized the game screen
/// 22/08/2019: 10:48 - 11:52 - Edited play screen scaling and wall placing/removing effects as well as a cursor

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif

#include "Game.h"

int main()
{
	srand(static_cast<unsigned>(time(nullptr)));
	Game game;
	game.run();

	return 0;
}
