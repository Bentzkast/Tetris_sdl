#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "constant.h"
#pragma once
// constant

struct NVector2
{
	int x;
	int y;
};

class Game
{
public:
	Game();
	bool Init();
	bool LoadAsset();
	void RunLoop();
	void Close();

private:
	void processInput();
	void updateGame();
	void renderDisplay();
	SDL_Texture* loadTexture(std::string path);
	SDL_Texture* loadTextureFromString(std::string text, SDL_Color textColor);
	void drawBlockAt(const NVector2& fieldPos, int x, int y, Color color);


	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	TTF_Font* mFont;
	SDL_Texture* mFontTexture;
	bool misRunning;
	Uint32 mTicksCount;
	bool mControl[ControlSize];
	int mScore;
};

class Tetris
{
public:
	Tetris();
	void NewPlayingField();
	unsigned char GetFieldAt(int x, int y) const;
	wchar_t  GetCurrentTetromino(int x, int y) const;
	BlockType GetCurrentBlockType();
	NVector2 const GetCurrentPiecePos() const;
	void Update(int& score, bool& isRunning, bool(&control)[Control::ControlSize]);


private:
	std::wstring mTetromino[7];
	// representation of the field in char array
	unsigned char mField[NFIELD_HEIGHT * NFIELD_HEIGHT];
	int mCurrentPieceIndex;
	int mCurrentPieceRotation;
	NVector2 mCurrentPiecePos;
	int mTetrisSpeed;
	int mTetrisTick;
	bool moveCurrentPiece(NVector2 v);
	void rotateCurrentPiece();
	bool CheckValidPos();
	std::vector<int> mLines;
	int delayForLineDect;
	int mPieceCount;
};
