#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
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

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	bool misRunning;
	Uint32 mTicksCount;
};

class Tetris
{
public:
	Tetris();
	void NewPlayingField();
	unsigned char GetFieldAt(int x, int y);
	//int rotate(NVector2 pos, int rotation);
	//bool DoesPieceFit(int )

	std::wstring tetromino[7];
	// representation of the field in char array
	unsigned char mField[NFIELD_HEIGHT * NFIELD_HEIGHT];

};
