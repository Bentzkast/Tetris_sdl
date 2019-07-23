#include "game.h"

Tetris gTetris;


Game::Game():
	mWindow{ nullptr },
	mRenderer{ nullptr },
	mTexture{ nullptr },
	misRunning{ true },
	mTicksCount{ 0 }
{

}

bool Game::Init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL could not initialize! SDL ERROR: %s\n", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Hello",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		NSCREEN_WIDTH,
		NSCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (mWindow == NULL)
	{
		SDL_Log("Window could not be created! SDL ERROR: %s\n", SDL_GetError());
		return false;
	}

	// Init renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == NULL)
	{
		printf("Renderer could not be initialize! SDL_image ERROR: %s\n", IMG_GetError());
		return false;
	}

	// Init renderer color
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Init Image loading
	int imgFlags = IMG_INIT_PNG;
	if ((IMG_Init(imgFlags) & imgFlags) == false)
	{
		printf("SDL image could not be initialize! SDL_image ERROR: %s\n", IMG_GetError());
		return false;
	}

	// get the surface contained by window
	//gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

void Game::Close() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	mRenderer = nullptr;
	mWindow = nullptr;

	SDL_Quit();
}

// Load Asset 
bool Game::LoadAsset() {
	if ((mTexture = loadTexture("asset/test.bmp")) == NULL) {
		return false;
	}

	gTetris.NewPlayingField();

	
	return true;
}


void Game::RunLoop() {
	Tetris tetris;
	tetris.NewPlayingField();

	while (misRunning == true)
	{
		processInput();
		updateGame();
		renderDisplay();
	}
}

void Game::processInput() {
	SDL_Event evt;

	while (SDL_PollEvent(&evt) != 0)
	{
		if (evt.type == SDL_QUIT)
		{
			misRunning = false;
		}
		else if (evt.type == SDL_KEYDOWN)
		{
			switch (evt.key.keysym.sym)
			{
			case SDLK_DOWN:
				SDL_Log("Down!\n");
				break;
			case SDLK_LEFT:
				SDL_Log("Left!\n");
				break;
			case SDLK_RIGHT:
				SDL_Log("Right!\n");
				break;
			case SDLK_SPACE:
				SDL_Log("Rotate!\n");
				break;
			case SDLK_ESCAPE:
				misRunning = false;
			}
		}
	}
}

void Game::updateGame() {
	float deltaTime = (SDL_GetTicks() - mTicksCount / 1000.0f);
	deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
	mTicksCount = SDL_GetTicks();

	// frame limiting to 60fps 16ms per-frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
}

void Game::renderDisplay() {

	SDL_SetRenderDrawColor(mRenderer, 0, 111, 170, 207);
	SDL_RenderClear(mRenderer);

	NVector2 fieldPos{ NSCREEN_WIDTH / 4 , NSCREEN_HEIGHT / 4 };

	// Draw the playing field frame
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	for (int x = 0; x < NFIELD_WIDTH; x++)
	{
		for (int y = 0; y < NFIELD_HEIGHT; y++)
		{
			switch (gTetris.GetFieldAt(x, y))
			{
				case BlockType::Wall:
				{
					SDL_Rect block{
						fieldPos.x + x * NBLOCK_SIZE_PXL,
						fieldPos.y + y * NBLOCK_SIZE_PXL,
						NBLOCK_SIZE_PXL,
						NBLOCK_SIZE_PXL };
										SDL_RenderFillRect(mRenderer, &block);
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	//SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
	SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::loadTexture(std::string path)
{
	SDL_Texture* pTexture = NULL;
	SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
	if (pLoadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image ERROR: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}
	// convert to screen format
	pTexture = SDL_CreateTextureFromSurface(mRenderer, pLoadedSurface);
	SDL_FreeSurface(pLoadedSurface);

	if (pTexture == NULL)
	{
		printf("Unable to create texture from image %s! SDL ERROR: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	return pTexture;
}

Tetris::Tetris() : mField{ {0} } {
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"....");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L".X..");
	tetromino[4].append(L".X..");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"..X.");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L"..X.");
	tetromino[6].append(L"....");
}

void Tetris::NewPlayingField() {
	for (int x = 0; x < NFIELD_WIDTH; x++)
	{
		for (int y = 0; y < NFIELD_HEIGHT; y++)
		{
			if (x == 0 || y == NFIELD_HEIGHT - 1 || x == NFIELD_WIDTH - 1)
			{
				mField[y * NFIELD_WIDTH + x] = BlockType::Wall;
			}
			else {
				mField[y * NFIELD_WIDTH + x] = 0;
			}
		}
	}
}


unsigned char Tetris::GetFieldAt(int x, int y){
	return mField[y * NFIELD_WIDTH + x];
}