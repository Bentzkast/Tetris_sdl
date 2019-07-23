#pragma once
const int NSCREEN_WIDTH = 1024;
const int NSCREEN_HEIGHT = 768;
const int NTETROMINO_SIZE = 4;

const int NFIELD_WIDTH = 12;
const int NFIELD_HEIGHT = 18;

const int NBLOCK_SIZE_PXL = 30;

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