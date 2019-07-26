#include "game.h"

Tetris gTetris;


Game::Game():
	mWindow{ nullptr },
	mRenderer{ nullptr },
	mTexture{ nullptr },
	mFont{ nullptr },
	mFontTexture{ nullptr}, 
	misRunning{ true },
	mTicksCount{ 0 },
	mControl{ {false}},
	mScore{ 100 } 
{
	mControl[Control::RotateUp] = true;
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

	if (TTF_Init() == -1) {
		SDL_Log("SDL ttf could not be initialize! SDL_ttf ERROR: %s\n", TTF_GetError());
		return false;
	}


	// get the surface contained by window
	//gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

void Game::Close() {
	TTF_CloseFont(mFont);
	mFont = nullptr;
	if (mFontTexture != nullptr) {
		SDL_DestroyTexture(mFontTexture);
	}

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	mRenderer = nullptr;
	mWindow = nullptr;


	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Load Asset 
bool Game::LoadAsset() {
	if ((mTexture = loadTexture("asset/test.bmp")) == NULL) {
		return false;
	}

	mFont = TTF_OpenFont("asset/Pixeled.ttf", 20);
	if (mFont == nullptr) {
		SDL_Log("Failed to load Pixeld font! SDL_ttf Error %s\n", TTF_GetError());
		return false;
	}
	else {

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

	if (SDL_PollEvent(&evt) != 0)
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
				mControl[Control::Down] = true;
				break;
			case SDLK_LEFT:
				mControl[Control::Left] = true;
				break;
			case SDLK_RIGHT:
				mControl[Control::Right] = true;
				break;
			case SDLK_SPACE:
				SDL_Log("Space Down");
				mControl[Control::Rotate] = true;
				break;
			case SDLK_ESCAPE:
				misRunning = false;
			}
		}
		else if (evt.type == SDL_KEYUP)
		{
			switch (evt.key.keysym.sym)
			{
				case SDLK_SPACE:
				{
					mControl[Control::RotateUp] = true;
					mControl[Control::Rotate] = false;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < Control::RotateUp; i++)
		{
			mControl[i] = false;
		}
	}
}

void Game::updateGame() {
	float deltaTime = (SDL_GetTicks() - mTicksCount / 1000.0f);
	deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
	mTicksCount = SDL_GetTicks();
	// ---------------------------------------------------

	gTetris.Update(mScore, misRunning, mControl);

	// frame limiting to 60fps 16ms per-frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
}

void Game::drawBlockAt(const NVector2& fieldPos, int x, int y, Color color) {
	SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
	SDL_Rect block{
		fieldPos.x + x * NBLOCK_SIZE_PXL,
		fieldPos.y + y * NBLOCK_SIZE_PXL,
		NBLOCK_SIZE_PXL,
		NBLOCK_SIZE_PXL };

	SDL_RenderFillRect(mRenderer, &block);
}

void Game::renderDisplay() {

	SDL_SetRenderDrawColor(mRenderer, 0, 111, 170, 207);
	SDL_RenderClear(mRenderer);

	NVector2 fieldPos{ NSCREEN_WIDTH / 4 , NSCREEN_HEIGHT / 4 };

	// Draw the playing field frame
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 180);

	for (int x = 0; x < NFIELD_WIDTH; x++)
	{
		for (int y = 0; y < NFIELD_HEIGHT; y++)
		{
			switch (gTetris.GetFieldAt(x, y))
			{
				case BlockType::Purple:
				{
					drawBlockAt(fieldPos, x, y, COLOR_PURPLE);
					break;
				}
				case BlockType::Red:
				{
					drawBlockAt(fieldPos, x, y, COLOR_RED);
					break;
				}
				case BlockType::Yellow:
				{
					drawBlockAt(fieldPos, x, y, COLOR_YELLOW);
					break;
				}
				case BlockType::Green:
				{
					drawBlockAt(fieldPos, x, y, COLOR_GREEN);
					break;
				}
				case BlockType::Cyan:
				{
					drawBlockAt(fieldPos, x, y, COLOR_CYAN);
					break;
				}
				case BlockType::Blue:
				{
					drawBlockAt(fieldPos, x, y, COLOR_BLUE);
					break;
				}
				case BlockType::White:
				{
					drawBlockAt(fieldPos, x, y, COLOR_WHITE);
					break;
				}
				case BlockType::Wall:
				{
					drawBlockAt(fieldPos, x, y, COLOR_BLACK);
					break;
				}
				default:
				{

					break;
				}
			}
			SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 180);
			SDL_Rect block{
				fieldPos.x + x * NBLOCK_SIZE_PXL,
				fieldPos.y + y * NBLOCK_SIZE_PXL,
				NBLOCK_SIZE_PXL,
				NBLOCK_SIZE_PXL };

			SDL_RenderDrawRect(mRenderer, &block);
		}
	}
	NVector2 const piecePos = gTetris.GetCurrentPiecePos();
	// Draw Current piece
	for (int x = 0; x < NTETROMINO_SIZE; x++)
	{
		for (int y = 0; y < NTETROMINO_SIZE; y++)
		{
			if (gTetris.GetCurrentTetromino(x,y) == L'X') {
			
				switch (gTetris.GetCurrentBlockType())
				{
				case BlockType::Purple: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y),  COLOR_PURPLE);
					break;
				}
				case BlockType::Red: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_RED);
					break;
				}
				case BlockType::Yellow: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_YELLOW);
					break;
				}
				case BlockType::Green: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_GREEN);
					break;
				}
				case BlockType::Cyan: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_CYAN);
					break;
				}
				case BlockType::Blue: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_BLUE);
					break;
				}
				case BlockType::White: {
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_WHITE);
					break;
				}
				case BlockType::Wall:
				{
					drawBlockAt(fieldPos, (piecePos.x + x), (piecePos.y + y), COLOR_WHITE);
					break;
				}
				default:
					break;
				}
			}
		}
	}
	
	// UI Score
	SDL_Rect renderQuad = { 10, 10, 48, 56 };
	loadTextureFromString(std::to_string(mScore), SDL_Color{ 0, 0, 0 });
	SDL_RenderCopy(mRenderer, mFontTexture, NULL, &renderQuad);

	renderQuad = { 10, 200, 20, 56 };
	loadTextureFromString("1", SDL_Color{ 0, 0, 0 });
	SDL_RenderCopy(mRenderer, mFontTexture, NULL, &renderQuad);

	// UI Title
	renderQuad = { 200, 10, 208, 56 };
	if (loadTextureFromString("Tetris Clone", SDL_Color{ 0, 0, 0 }) == nullptr) {
		printf("Failed to load text texture!\n");
	}

	SDL_RenderCopy(mRenderer, mFontTexture, NULL, &renderQuad);

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

SDL_Texture* Game::loadTextureFromString(std::string text, SDL_Color textColor) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text.c_str(), textColor);
	if (textSurface == nullptr) {
		SDL_Log("Unable to render text surface! SDL ERROR: %s\n", SDL_GetError());
		return nullptr;
	}
	mFontTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	if (mFontTexture == nullptr) {
		SDL_Log("Unable to create texture from surface! SDL ERROR: %s\n", SDL_GetError());
		return nullptr;
	}

	//SDL_Log("%d, %d ", textSurface->w, textSurface->h);
}

