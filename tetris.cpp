#include "game.h"

Tetris::Tetris() :
	mField{ {0} },
	mCurrentPieceIndex{ 1 },
	mCurrentPieceRotation{ 0 },
	mTetrisSpeed{ NTETRIS_SPEED },
	mTetrisTick{ 0 },
	mLines{ },
	delayForLineDect{ 0 } ,
	mPieceCount{ 0 }{

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
				mField[y * NFIELD_WIDTH + x] = BlockType::Empty;
			}
		}
	}
}


unsigned char Tetris::GetFieldAt(int x, int y) const {
	return mField[y * NFIELD_WIDTH + x];
}


BlockType Tetris::GetCurrentBlockType() {
	return static_cast<BlockType>(mCurrentPieceIndex + 1);
}

wchar_t Tetris::GetCurrentTetromino(int x, int y) const {
	switch (mCurrentPieceRotation % 4)
	{
	case 0:
		return (mTetromino[mCurrentPieceIndex])[y * NTETROMINO_SIZE + x];
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

void Tetris::Update(int &score, bool& isRunning, bool(&control)[Control::ControlSize]) {
	mTetrisTick++;

	if (delayForLineDect > 0) {
		delayForLineDect--;
		if (delayForLineDect <= 0) {

			for (auto& line : mLines)
			{
				for (int y = line; y > 0; y--)
				{
					// not wall
					for (int x = 1; x < NFIELD_WIDTH - 1; x++)
					{
						// shift down by 1 to fill gap
						mField[y * NFIELD_WIDTH + x] = mField[(y - 1) * NFIELD_WIDTH + x];
						mField[x] = BlockType::Empty;
					}
				}
			}
			mLines.clear();

			delayForLineDect = 0;
		}
	}


	if (control[Control::Left]) {
		moveCurrentPiece(NVector2{ -1,0 });
	}
	if (control[Control::Right]) {
		moveCurrentPiece(NVector2{ 1, 0 });
	}

	if (control[Control::Down]) {
		moveCurrentPiece(NVector2{ 0, 1 });
	}

	if (control[Control::Rotate] && control[Control::RotateUp]) {
		rotateCurrentPiece();
		control[Control::RotateUp] = false;
	}


	if (mTetrisTick == mTetrisSpeed) {
		mTetrisTick = 0;

		// tick down
		if (moveCurrentPiece(NVector2{ 0, 1 }) == false) {
			// cannot move down any more, lock piece
			for (int x = 0; x < NTETROMINO_SIZE; x++)
			{
				for (int y = 0; y < NTETROMINO_SIZE; y++)
				{
					if (GetCurrentTetromino(x, y) != L'.') {
						mField[(mCurrentPiecePos.y + y) * NFIELD_WIDTH + (mCurrentPiecePos.x + x)] = GetCurrentBlockType();
					}
				}
			}


			// check if a line exist for the 4 line that the new piece is added
			for (int y = 0; y < NTETROMINO_SIZE; y++)
			{
				// is the current line exist in the playin field
				if (mCurrentPiecePos.y + y < NFIELD_HEIGHT - 1) {
					// iterate inside of the block boundry, not wall
					bool isLine = true;

					for (int x = 1; x < NFIELD_WIDTH - 1; x++)
					{
						isLine &=
							(mField[(mCurrentPiecePos.y + y) * NFIELD_WIDTH
								+ x] != BlockType::Empty);
					}

					if (isLine) {
						for (int x = 1; x < NFIELD_WIDTH - 1; x++)
						{
							mField[(mCurrentPiecePos.y + y) * NFIELD_WIDTH
								+ x] = BlockType::Wall;

						}
						mLines.push_back((mCurrentPiecePos.y + y));
						delayForLineDect = 30;
					}
				}
			}

			score += 25;
			if (!mLines.empty() ) score += (1 << mLines.size()) * 100;
			// pick new piece
			mPieceCount++;
			if (mPieceCount % 10 == 0) {
				if (mTetrisSpeed > 10) mTetrisSpeed--;
			}
			mCurrentPiecePos = NVector2{ NFIELD_WIDTH / 2, 0 };
			mCurrentPieceRotation = 0;
			mCurrentPieceIndex = rand() % 7;



			if (moveCurrentPiece(NVector2{ 0, 0 }) == false) {
				isRunning = false;
			}
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