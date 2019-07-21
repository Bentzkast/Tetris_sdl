// Include dependencies
#include <SDL.h>
#include <stdio.h>

// Screen dimension constant

const int NSCREEN_WIDTH = 640;
const int NSCREEN_HEIGHT = 480;


bool initEngine();
bool loadAsset();
void closeEngine();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gSplashScreen = NULL;


int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	// Initialize

	if (initEngine() == false)
	{
		printf("ENGINE - failed to initialize!\n");
		return 0;
	}
	
	if (loadAsset() == false)
	{
		printf("ENGINE - failed to load asset!\n");
		return 0;
	}

	// copy splashscreen 
	SDL_BlitSurface(gSplashScreen, NULL, gScreenSurface, NULL);
	

	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x11, 0x11, 0x11));
	SDL_UpdateWindowSurface(gWindow); // apply the draw call
	SDL_Delay(2000); // 2 seconds

	closeEngine();
	
	return 0;
}

bool initEngine()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	// Create Window 
	gWindow = SDL_CreateWindow(
		"Hello",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		NSCREEN_WIDTH,
		NSCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	// fail check
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	// get the surface contained by window
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

bool loadAsset()
{
	// load surface from bmp file
	gSplashScreen = SDL_LoadBMP("asset/test.bmp");
	if (gSplashScreen == NULL)
	{
		printf("Unable to load image %s! SDL_ERROR: %s\n" ,"asset/test.bmp", SDL_GetError());
		return false;
	}
	return true;

}

void closeEngine()
{
	SDL_FreeSurface(gSplashScreen);
	gSplashScreen = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}
