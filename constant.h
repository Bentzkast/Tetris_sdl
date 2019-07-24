#pragma once
const int NSCREEN_WIDTH = 1024;
const int NSCREEN_HEIGHT = 768;
const int NTETROMINO_SIZE = 4;

const int NFIELD_WIDTH = 12;
const int NFIELD_HEIGHT = 18;

const int NBLOCK_SIZE_PXL = 30;

const int NTETRIS_SPEED = 20;

struct Color {
	int r;
	int g;
	int b;
	int a;
};

const Color COLOR_BLACK{ 23, 23, 23, 255 };
const Color COLOR_PURPLE{ 147, 112, 219, 255 };
const Color COLOR_RED{ 255, 0, 0, 255 };
const Color COLOR_YELLOW{ 240, 230, 140, 255 };
const Color COLOR_GREEN{ 0, 128, 0, 255 };
const Color COLOR_CYAN{ 32, 178, 170, 255 };
const Color COLOR_BLUE{ 100, 149, 237, 255 };
const Color COLOR_WHITE{ 240, 240, 240, 255 };


enum BlockType {
	Empty,
	Purple,
	Red,
	Yellow,
	Green,
	Cyan,
	Blue,
	White,
	Wall
};

enum Control {
	Left,
	Right,
	Down,
	Rotate,
	RotateUp,
	ControlSize
};