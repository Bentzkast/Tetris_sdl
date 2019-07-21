// Include dependencies
#include <SDL.h>
#include <stdio.h>
#include <string>

// constant
const int NSCREEN_WIDTH = 720;
const int NSCREEN_HEIGHT = 480;

// Funtions
bool initEngine();
bool loadAsset();
void closeEngine();
void gameLoop();
SDL_Surface* loadSurface(std::string path);

// globals
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gSplashScreen = NULL;


int main(int argc, char* args[])
{
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

	// copy & stretch splashscreen to screen surface
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = NSCREEN_WIDTH;
	stretchRect.h = NSCREEN_HEIGHT;


	SDL_BlitScaled(gSplashScreen, NULL, gScreenSurface, &stretchRect);
	//SDL_BlitSurface(gSplashScreen, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow); // apply the draw call
	SDL_Delay(1000); // 1 second
	gameLoop();

	//std::thread t = std::thread(&gameThread);
	//t.join();

	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x11, 0x11, 0x11));

	closeEngine();
	
	return 0;
}

void gameLoop()
{
	bool bGameOver = false;
	SDL_Event evt;
	while (bGameOver == false)
	{
		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT)
			{
				bGameOver = true;
			}
			else if(evt.type == SDL_KEYDOWN)
			{
				switch (evt.key.keysym.sym)
				{
					case SDLK_UP:
						printf("Up!\n");
						break;
					case SDLK_DOWN:
						printf("Down!\n");
						break;
					case SDLK_LEFT:
						printf("Left!\n");
						break;
					case SDLK_RIGHT:
						printf("Right!\n");
						break;
				}
			}
		}
	}
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

SDL_Surface* loadSurface(std::string path)
{
	SDL_Surface* pOptSurface = NULL;
	SDL_Surface* pLoadedSurface = SDL_LoadBMP(path.c_str());
	if (pLoadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_ERROR: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	// convert to screen format
	pOptSurface = SDL_ConvertSurface(pLoadedSurface, gScreenSurface->format, 0);
	SDL_FreeSurface(pLoadedSurface);

	if (pOptSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL_ERROR: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}


	return pOptSurface;
}


bool loadAsset()
{
	// load surface from bmp file
	if ((gSplashScreen = loadSurface("asset/test.bmp")) == NULL) {
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
