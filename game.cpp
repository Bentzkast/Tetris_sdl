#include "game.h"

Tetris gTetris;


Game::Game():
	mWindow{ nullptr },
	mRenderer{ nullptr },
	mTexture{ nullptr },
	misRunning{ true },
	mTicksCount{ 0 },
	mControl{ {false} }
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

	gTetris.Update(mControl);

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
				default:
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

Tetris::Tetris() :
	mField{ {0} },
	mCurrentPieceIndex{ 1 },
	mCurrentPieceRotation{ 0 },
	mTetrisSpeed{ NTETRIS_SPEED },
	mTetrisTick{ 0 }{

	mCurrentPiecePos = { NFIELD_WIDTH / 2, 0 };

	mTetromino[0].append(L"..X.");
	mTetromino[0].append(L"..X.");
	mTetromino[0].append(L"..X.");
	mTetromino[0].append(L"..X.");

	mTetromino[1].append(L"..X.");
	mTetromino[1].append(L".XX.");
	mTetromino[1].append(L".X..");
	mTetromino[1].append(L"....");

	mTetromino[2].append(L".X..");
	mTetromino[2].append(L".XX.");
	mTetromino[2].append(L"..X.");
	mTetromino[2].append(L"....");

	mTetromino[3].append(L"....");
	mTetromino[3].append(L".XX.");
	mTetromino[3].append(L".XX.");
	mTetromino[3].append(L"....");

	mTetromino[4].append(L"....");
	mTetromino[4].append(L".XX.");
	mTetromino[4].append(L".X..");
	mTetromino[4].append(L".X..");

	mTetromino[5].append(L"....");
	mTetromino[5].append(L".XX.");
	mTetromino[5].append(L"..X.");
	mTetromino[5].append(L"..X.");

	mTetromino[6].append(L"..X.");
	mTetromino[6].append(L".XX.");
	mTetromino[6].append(L"..X.");
	mTetromino[6].append(L"....");
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


unsigned char Tetris::GetFieldAt(int x, int y) const{
	return mField[y * NFIELD_WIDTH + x];
}


BlockType Tetris::GetCurrentBlockType() {
	return static_cast<BlockType>(mCurrentPieceIndex + 1);
}

wchar_t Tetris::GetCurrentTetromino(int x, int y) const {
	switch (mCurrentPieceRotation % 4)
	{
	case 0:
		return (mTetromino[mCurrentPieceIndex])[y * NTETROMINO_SIZE +  x];
	case 1:
		return (mTetromino[mCurrentPieceIndex])[12 + y - (x * NTETROMINO_SIZE)];
	case 2:
		return (mTetromino[mCurrentPieceIndex])[15 - (y * NTETROMINO_SIZE) - x];
	case 3:
		return (mTetromino[mCurrentPieceIndex])[3 - y + (x * NTETROMINO_SIZE)];
	}

	return 0;
}

NVector2 const Tetris::GetCurrentPiecePos() const {
	return mCurrentPiecePos;
}

bool Tetris::moveCurrentPiece(NVector2 v) {
	mCurrentPiecePos.x += v.x;
	mCurrentPiecePos.y += v.y;

	if (CheckValidPos() == false) {
		mCurrentPiecePos.x -= v.x;
		mCurrentPiecePos.y -= v.y;
		return false;
	}
	return true;
}

void Tetris::rotateCurrentPiece() {
	mCurrentPieceRotation += 1;

	if (CheckValidPos() == false) {
		mCurrentPieceRotation -= 1;
		return;
	}
}

void Tetris::Update(bool (&control)[Control::ControlSize]) {
	mTetrisTick++;


	if (control[Control::Left]) {
		moveCurrentPiece(NVector2{ -1,0 });
	}
	if (control[Control::Right]) {
		moveCurrentPiece(NVector2{ 1, 0 });
	}

	if (control[Control::Rotate] && control[Control::RotateUp]) {
		rotateCurrentPiece();
		control[Control::RotateUp] = false;
	}


	if (mTetrisTick == mTetrisSpeed) {
		mTetrisTick = 0;

		if (moveCurrentPiece(NVector2{ 0, 1 }) == false) {
			for (int x = 0; x < NTETROMINO_SIZE; x++)
			{
				for (int y = 0; y < NTETROMINO_SIZE; y++)
				{
					if (GetCurrentTetromino(x, y) != L'.') {
						mField[(mCurrentPiecePos.y + y) * NFIELD_WIDTH + (mCurrentPiecePos.x + x)] = gTetris.GetCurrentBlockType();
					}
				}
			}

			mCurrentPiecePos = NVector2{ NFIELD_WIDTH / 2, 0 };
			mCurrentPieceRotation = 0;
			mCurrentPieceIndex = rand() % 7;
		}
	}
}

bool Tetris::CheckValidPos() {
	for (int x = 0; x < NTETROMINO_SIZE; x++)
	{
		for (int y = 0; y < NTETROMINO_SIZE; y++)
		{
			wchar_t block = GetCurrentTetromino(x, y);
			int blockIndexInFieldCoord = 
				(mCurrentPiecePos.y + y) * NFIELD_WIDTH
				+ (mCurrentPiecePos.x + x);	

			// only check block if it is inside the playing bound
			if (mCurrentPiecePos.x + x >= 0 && mCurrentPiecePos.x + x < NFIELD_WIDTH) {
				if (mCurrentPiecePos.y + y >= 0 && mCurrentPiecePos.y + y < NFIELD_HEIGHT) {
					if (block != L'.' && mField[blockIndexInFieldCoord] != BlockType::Empty) {
						return false;
					}
				}
			}
		}
	}
	return true;
}