#pragma once

#define STRSIZE 50

#define XRES 1280
#define YRES 720

struct HANDLES
{
	HBITMAP hbmp, hOldbmp;
	HDC dc;
	BITMAP bit;
	POINT pt;

	BLENDFUNCTION bf;
};

struct UPDATE
{
	DWORD mStartTick;
	DWORD mEndTick;

	DWORD aStartTick;
	DWORD aEndTick;

	int MaxFrame;
	int frame;
	int size;

	BOOL bTrigger;
};

enum GAME_STATE
{
	TITLE,
	GAME_READY,
	GAME_EASY,
	GAME_NORMAL,
	GAME_HARD,
	GAME_MAIN,
	STAGE_CLEAR,
	GAME_CLEAR,
	GAME_OVER,
	RANKING
};
