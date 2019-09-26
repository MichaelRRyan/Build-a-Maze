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
/// 22/08/2019: 21:30 - 22:04 - Fixed mouse position scaling, edited cursor to only show over tiles that can be edited
/// 23/08/2019: 13:15 - 14:24 - Changed screen size to a 720p and added a basic shop menu
/// 24/08/2019: 11:43 - 13:02 - Tidied some code and changed it to allow for more tiles. Added plant tile that makes AI move slower
/// 27/08/2019: 14:16 - 14:35 - Fixed a bug with maze generation that made the maze unsolvable. Made AI disappear after completing the maze
/// 27/08/2019: 18:50 - 19:21 - Added game states (Simulation &  BuildMode) and gave the user the ability to switch between them. Sped up AI movement. Added text to display how many AI are left in the maze
/// 28/08/2019: 13:19 - 13:34 - Minor tweaks to code
/// 30/08/2019: 12:08 - 12:45 - Gave AI the ability to see the goal within 3 tiles away to stop them walking right past it. Player now earns money at the end of a sim. Added zero in front of single digit second values display for 'time to complete'
/// 30/08/2019: 22:40 - 23:40 - Moved sim details to side panel and made them viewable after sim ends. Stopped rendering constuction shop panel while in sim mode. Tidied and commented some code. Added pause screen
/// 31/08/2019: 01:41 - 02:20 - Added basic title screen
/// 02/09/2019: 01:47 - 02:17 - Added simulation stats icons and shortened the side panel during sim mode
/// 04/09/2019: 19:04 - 19:44 - Slightly changed title screen, side panel and pause screen a bit.
/// 25/09/2019: 13:50 - 15:00 - Added basic controller support and added three new cursors
/// 26/09/2019: 12:30 - 12:50 - Added functions to clean up code and added comments


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
