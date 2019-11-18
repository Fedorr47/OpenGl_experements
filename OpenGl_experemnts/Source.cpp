#include <iostream>

// Other includes
#include "game_logic.h"

// The MAIN function, from here we start the application and run the game loop
int main()
{
	game_ptr.reset(new GameEntry());
	game_ptr->GameLoop();
	return 0;
}

