/// Basic Game
/// @Author Michael Rainsford Ryan
/// @Date 19/08/2019
/// Work Sessions:
/// 19/08/2019 - 16:34 - Set up basic game files and created project
/// 19/08/2019 - 16:34 - 18:02 - created and set up a 2D array and rendered. Started on maze generation algorithm
/// 19/08/2019 - 18:33 - 19:35 - Finished maze generation algorithm and added basic placehold textures
/// 20/08/2019 - 11:14 - 11:52 - Added basic AI
/// 20/08/2019 - 12:44 - 12:50 - Edited rendering order of the tiles and AI to add slight 3D overlap effect

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
