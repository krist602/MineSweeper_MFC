#pragma once

#include "framework.h"

class Game
{
private:
	HANDLES m_Base;

	//화면
	HANDLES m_BG;
	HANDLES m_Menu;
	HANDLES m_Title;
	HANDLES m_Select;
	HANDLES m_Dif;
	HANDLES m_Board[32][18];
	HANDLES m_Smile;

	//update
	UPDATE m_uSelect;


	RECT m_rtClient;
	int m_nGameState;
	int m_nScore;
	float m_nTime;
	WCHAR m_chScore[STRSIZE];

	int v_select = 0;

	//지뢰찾기 변수
	pair<int, bool> board[32][18]; // 게임 보드. 지뢰 = -1, 주위에 지뢰가 있는 숫자만큼 숫자 배분. bool값이 TRUE 라면 Visible True. 깃발이면 += 110으로 해두자.
	int numMine = 0; // 지뢰의 수
	int numFlag = 0; // 깃발의 수
	int numDig = 1; // Dig의 수
	int X = 0;
	int Y = 0;
	int xleft = 0;
	int xright = 0;
	int ytop = 0;
	int ybottom = 0;

public:
	Game();
	~Game();

	void GameLoop(HDC hdc);
	void DrawAll(HDC hdc);
	void Update();

	//init
	void InitAll();
	void InitResource();
	void InitLevel();
	void InitMine();

	void Realease(HWND hWnd);

	//draw
	void DrawBG();
	void DrawTitle();
	void DrawMenu();
	void DrawSelect();
	void DrawDif();
	void DrawBoard();
	void DrawAll(); //클리어 했을 때
	void DrawMine(); //지뢰가 터졌을 때
	void DrawNum(int x, int y); //숫자일 때 숫자 출력하기
	void DrawSmile();
	void DrawDead();
	void DrawCool();

	//update
	BOOL UpdateCheck(UPDATE& up, DWORD tick);
	void UpdateSelect();
	void UpSelect();
	void DownSelect();

	//select
	void SelectMenu();
	void SelectDif();
	int GetSelect() const;
	int GetGameState() const;

	//util
	void Load(HANDLES& h, LPCWSTR name);
	RECT& GetRect();

	//event
	void Dig(int xpos, int ypos);
	void Flag(int xpos, int ypos);
	int GetDig() const;
	int GetFlag() const;
	void Click(int xpos, int ypos);
	void Reset();
};

