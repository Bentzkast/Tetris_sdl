// Include dependencies

#include "game.h"


int main(int argc, char* args[])
{
	Game game;


	if (game.Init() == false)
	{
		printf("GAME - failed to initialize!\n");
		return 0;
	}

	if (game.LoadAsset() == false)
	{
		printf("GAME - failed to load asset!\n");
		return 0;
	}

	game.RunLoop();
	game.Close();

	return 0;
}

