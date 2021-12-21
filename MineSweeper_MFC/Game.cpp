#include "Game.h"

void Game::InitAll()
{
	m_uSelect.bTrigger = TRUE;
}

void Game::InitResource()
{
	m_Base.hbmp = (HBITMAP)LoadImage(NULL, L"image/base.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_Base.dc = CreateCompatibleDC(NULL);
	m_Base.hOldbmp = (HBITMAP)SelectObject(m_Base.dc, m_Base.hbmp);
	GetObject(m_Base.hbmp, sizeof(BITMAP), &m_Base.bit);

	Load(m_BG, L"image/title.bmp");
	Load(m_Title, L"image/minesweeper.bmp");
	Load(m_Menu, L"image/menu.bmp");
	Load(m_Select, L"image/select.bmp");
	Load(m_Dif, L"image/dif.bmp");
	Load(m_Home, L"image/home.bmp");

	Load(m_Cool, L"image/cool.bmp");
	Load(m_Dead, L"image/dead.bmp");
	
	hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"D2Cording");
	hOldFont = (HFONT)SelectObject(m_Base.dc, hFont);
}

void Game::InitLevel()
{
	switch (m_nGameState)
	{
	case GAME_EASY:
		X = 9;
		Y = 9;
		numMine = 10;
		break;
	case GAME_NORMAL:
		X = 16;
		Y = 16;
		numMine = 40;
		break;
	case GAME_HARD:
		X = 30;
		Y = 16;
		numMine = 99;
		break;
	default:
		break;
	}

	numFlag = 0;
	numDig = X * Y - numMine;

	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 18; j++)
			board[i][j] = { 0, false };

	Load(m_Smile, L"image/smile.bmp");
	for (int i = 0; i < X; i++)
		for (int j = 0; j < Y; j++)
			Load(m_Board[i][j], L"image/sqaure.bmp");

	InitMine();

}

void Game::InitMine()
{
	int x = 0, y = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < numMine; i)
	{
		x = rand() % X;
		y = rand() % Y;
		if (board[x][y].first == -1)
			continue;
		else
			board[x][y].first = -1;

		if (x == 0)
		{
			if (y == 0)
			{
				for (int j = x; j <= x + 1; j++)
				{
					for (int k = y; k <= y + 1; k++)
					{
						if (board[j][k].first == -1)
							continue;
						else
							board[j][k].first += 1;
					}
				}
			}
			else
			{
				for (int j = x; j <= x + 1; j++)
				{
					for (int k = y - 1; k <= y + 1; k++)
					{
						if (board[j][k].first == -1)
							continue;
						else
							board[j][k].first += 1;
					}
				}
			}
		}
		else if (y == 0)
		{
			for (int j = x - 1; j <= x + 1; j++)
			{
				for (int k = y; k <= y + 1; k++)
				{
					if (board[j][k].first == -1)
						continue;
					else
						board[j][k].first += 1;
				}
			}
		}
		else
		{
			for (int j = x - 1; j <= x + 1; j++)
			{
				for (int k = y - 1; k <= y + 1; k++)
				{
					if (board[j][k].first == -1)
						continue;
					else
						board[j][k].first += 1;
				}
			}
		}
		i++;
	}
}

void Game::DrawBG()
{
	StretchBlt(m_Base.dc, 0, 0, XRES + 150, YRES, m_BG.dc, 0, 0, XRES + 150, YRES, SRCCOPY);
}

void Game::DrawTitle()
{
	int x = m_rtClient.right / 2 - m_Title.bit.bmWidth / 2 - 50;
	int y = 50;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Title.bit.bmWidth,
		m_Title.bit.bmHeight,
		m_Title.dc,
		0, 0,
		m_Title.bit.bmWidth,
		m_Title.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawMenu()
{
	int x = m_rtClient.right / 2 - m_Menu.bit.bmWidth / 2;
	int y = 350;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Menu.bit.bmWidth,
		m_Menu.bit.bmHeight,
		m_Menu.dc,
		0, 0,
		m_Menu.bit.bmWidth,
		m_Menu.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawSelect()
{
	if (m_uSelect.bTrigger == FALSE) return;

	int x = m_rtClient.right / 2 - m_Menu.bit.bmWidth / 2 - m_Select.bit.bmWidth;
	int y = 350 + 10 + v_select * 95;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Select.bit.bmWidth,
		m_Select.bit.bmHeight,
		m_Select.dc,
		0, 0,
		m_Select.bit.bmWidth,
		m_Select.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawDif()
{
	int x = m_rtClient.right / 2 - m_Dif.bit.bmWidth / 2;
	int y = 350;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Dif.bit.bmWidth,
		m_Dif.bit.bmHeight,
		m_Dif.dc,
		0, 0,
		m_Dif.bit.bmWidth,
		m_Dif.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawBoard()
{
	int x = m_rtClient.right / 2 - X * 15;
	int y = m_rtClient.bottom / 2 - Y * 15;
	xleft = x;
	xright = x + X * 30;
	ytop = y;
	ybottom = y + Y * 30;

	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			TransparentBlt(m_Base.dc,
				x + i * 30,
				y + j * 30,
				m_Board[i][j].bit.bmWidth,
				m_Board[i][j].bit.bmHeight,
				m_Board[i][j].dc,
				0, 0,
				m_Board[i][j].bit.bmWidth,
				m_Board[i][j].bit.bmHeight,
				RGB(255, 0, 255)
			);
		}
	}
}


BOOL Game::UpdateCheck(UPDATE& up, DWORD tick)
{
	up.mEndTick = GetTickCount();
	if (up.mEndTick - up.mStartTick > tick)
	{
		up.mStartTick = up.mEndTick;
		return TRUE;
	}
	return FALSE;
}

void Game::UpdateSelect()
{
	if (UpdateCheck(m_uSelect, 800) == TRUE)
	{
		m_uSelect.bTrigger = !m_uSelect.bTrigger;
	}
}

void Game::UpSelect()
{
	m_uSelect.bTrigger = TRUE;
	if (v_select == 0)
	{
		v_select = 2;
		return;
	}
	v_select -= 1;
	return;
}

void Game::DownSelect()
{
	m_uSelect.bTrigger = TRUE;
	if (v_select == 2)
	{
		v_select = 0;
		return;
	}
	v_select += 1;
	return;
}

void Game::SelectMenu()
{
	switch (v_select)
	{
	case 0: //GameStart
		m_nGameState = GAME_READY;
		break;
	case 1: //Ranking
		m_nGameState = RANKING;
		break;
	case 2: //Exit
		break;
	}
}

void Game::SelectDif()
{
	switch (v_select)
	{
	case 0:
		m_nGameState = GAME_EASY;
		level = m_nGameState;
		InitLevel();
		break;
	case 1:
		m_nGameState = GAME_NORMAL;
		level = m_nGameState;
		InitLevel();
		break;
	case 2:
		m_nGameState = GAME_HARD;
		level = m_nGameState;
		InitLevel();
		break;
	}

}

int Game::GetSelect() const
{
	return v_select;
}

int Game::GetGameState() const
{
	return m_nGameState;
}

void Game::Load(HANDLES& h, LPCWSTR name)
{
	h.hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	if (h.hbmp == NULL)
	{
		MessageBox(NULL, name, L"비트맵 로딩 에러", MB_OK);
		exit(0);
	}
	h.dc = CreateCompatibleDC(m_Base.dc);
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);
	GetObject(h.hbmp, sizeof(BITMAP), &h.bit);
}

RECT& Game::GetRect()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_rtClient;
}

void Game::DrawNum(int x, int y)
{
	switch (board[x][y].first)
	{
	case 1:
		Load(m_Board[x][y], L"image/sqaure2_1.bmp");
		break;
	case 2:
		Load(m_Board[x][y], L"image/sqaure2_2.bmp");
		break;
	case 3:
		Load(m_Board[x][y], L"image/sqaure2_3.bmp");
		break;
	case 4:
		Load(m_Board[x][y], L"image/sqaure2_4.bmp");
		break;
	case 5:
		Load(m_Board[x][y], L"image/sqaure2_5.bmp");
		break;
	case 6:
		Load(m_Board[x][y], L"image/sqaure2_6.bmp");
		break;
	default:
		break;
	}

	return;
}

void Game::DrawSmile()
{
	int x = m_rtClient.right / 2 - 20;
	int y = m_rtClient.bottom / 2 - Y * 15 - 60;
	m_SmileRT = { x, y , x + 40, y + 40 };
	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Smile.bit.bmWidth + 10,
		m_Smile.bit.bmHeight + 10,
		m_Smile.dc,
		0, 0,
		m_Smile.bit.bmWidth,
		m_Smile.bit.bmHeight,
		RGB(255, 0, 255)
	);
	xTime = x + 60;
	yTime = y;

}

void Game::DrawHome()
{
	int x = 10;
	int y = 10;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Home.bit.bmWidth,
		m_Home.bit.bmHeight,
		m_Home.dc,
		0, 0,
		m_Home.bit.bmWidth,
		m_Home.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawTime()
{
	SetBkMode(m_Base.dc, TRANSPARENT);
	

	if (m_bPlaying == FALSE)
	{
		SetTextColor(m_Base.dc, RGB(0, 0, 0));//글자색 변경- 검은색
		TextOut(m_Base.dc, xTime, yTime, L"0", 1);

		SetTextColor(m_Base.dc, RGB(255, 0, 0));//글자색 변경- 빨간색
		TextOut(m_Base.dc, xTime-2, yTime-2, L"0", 1);
		return;
	}

	size_t size = 0;
	wsprintf(m_chTime, L"%d", (int)(clock() - t_start) / CLOCKS_PER_SEC);
	StringCchLength(m_chTime, STRSIZE, &size);

	SetTextColor(m_Base.dc, RGB(0, 0, 0));//글자색 변경- 검은색
	TextOut(m_Base.dc, xTime, yTime, m_chTime, (int)size);

	SetTextColor(m_Base.dc, RGB(255, 0, 0));//글자색 변경- 빨간색
	TextOut(m_Base.dc, xTime - 2, yTime - 2, m_chTime, (int)size);
}

void Game::DrawnumFlag()
{
	SetBkMode(m_Base.dc, TRANSPARENT);

	size_t size = 0;
	wsprintf(m_chFlag, L"%d", (int)(numMine - numFlag));
	StringCchLength(m_chFlag, STRSIZE, &size);

	SetTextColor(m_Base.dc, RGB(0, 0, 0));//글자색 변경- 검은색
	TextOut(m_Base.dc, xTime - 120, yTime, m_chFlag, (int)size);

	SetTextColor(m_Base.dc, RGB(255, 0, 0));//글자색 변경- 빨간색
	TextOut(m_Base.dc, xTime - 122, yTime - 2, m_chFlag, (int)size);
}

void Game::DrawFinishTime()
{
	SetBkMode(m_Base.dc, TRANSPARENT);

	size_t size = 0;
	wsprintf(m_chTime, L"%d", (int)(t_finish - t_start) / CLOCKS_PER_SEC);
	StringCchLength(m_chTime, STRSIZE, &size);

	SetTextColor(m_Base.dc, RGB(0, 0, 0));//글자색 변경- 검은색
	TextOut(m_Base.dc, xTime, yTime, m_chTime, (int)size);

	SetTextColor(m_Base.dc, RGB(255, 0, 0));//글자색 변경- 빨간색
	TextOut(m_Base.dc, xTime - 2, yTime - 2, m_chTime, (int)size);
}

void Game::DrawDead()
{
	int x = m_rtClient.right / 2 - 20;
	int y = m_rtClient.bottom / 2 - Y * 15 - 60;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Dead.bit.bmWidth + 10,
		m_Dead.bit.bmHeight + 10,
		m_Dead.dc,
		0, 0,
		m_Dead.bit.bmWidth,
		m_Dead.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::DrawCool()
{
	int x = m_rtClient.right / 2 - 20;
	int y = m_rtClient.bottom / 2 - Y * 15 - 60;

	TransparentBlt(m_Base.dc,
		x,
		y,
		m_Cool.bit.bmWidth + 10,
		m_Cool.bit.bmHeight + 10,
		m_Cool.dc,
		0, 0,
		m_Cool.bit.bmWidth,
		m_Cool.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void Game::Click(int xpos, int ypos)
{
	if (PtInRect(&m_homeRT, { xpos, ypos }))
	{
		m_nGameState = TITLE;
		return;
	}


	if (PtInRect(&m_SmileRT, { xpos, ypos }))
	{
		//Reset();
		m_bPlaying = FALSE;
		b_Mine = FALSE;
		m_nGameState = level;
		InitLevel();
		//InitMine();
		return;
	}
	else if (xpos<xleft || xpos>xright || ypos<ytop || ypos>ybottom)
		return;
	else
	{
		if (m_bPlaying == FALSE)
		{
			m_bPlaying = TRUE;
			t_start = clock();
		}
		if (m_nGameState == GAME_OVER || m_nGameState == GAME_CLEAR)
			return;
		Dig(xpos, ypos);
	}
}

void Game::Reset()
{
	InitLevel();
}

void Game::canClear()
{
	if (numDig == 0)
	{
		m_bPlaying = FALSE;
		b_Cool = FALSE;
		t_finish = clock();
		m_nGameState = GAME_CLEAR;
	}
}

void Game::Dig(int xpos, int ypos)
{
	if (xpos<xleft || xpos>xright || ypos<ytop || ypos>ybottom)
		return;
	int x = 1;
	int y = 1;
	for (x = 1; x < X; x++)
		if (xpos < x * 30 + xleft)
			break;
	for (y = 1; y < Y; y++)
		if (ypos < y * 30 + ytop)
			break;
	x -= 1; //배열은 0,0 부터 시작
	y -= 1;
	if (board[x][y].second == true)
		return;
	if (board[x][y].first <= 10)
	{
		if (board[x][y].first == -1) //-1이라면
		{
			//지뢰가 터진다. //GAME_OVER
			Load(m_Board[x][y], L"image/sqaurebombmine.bmp");
			board[x][y].first = -10;
			t_finish = clock();
			m_bPlaying = FALSE;
			b_Dead = FALSE;
			m_nGameState = GAME_OVER;
			
			return;
		}
		else if (board[x][y].first == 0) //주위에 지뢰가 없다면
		{
			if (board[x][y].second == false)
			{
				numDig -= 1;
				board[x][y].second = true; // 본인을 0으로 만들고
				Load(m_Board[x][y], L"image/sqaure2.bmp");
			}
			for (int j = xpos - 30; j <= xpos + 30; j = j + 30)
			{
				for (int k = ypos - 30; k <= ypos + 30; k = k + 30)
				{
					Dig(j, k); //주위에 다시 탐색
				}
			}
		}
		else 
		{
			board[x][y].second = true;
			DrawNum(x, y);
			numDig -= 1;
			return;
		}
	}
	else if (board[x][y].first >= 100)
	{
		return;
	}
	return;
}

void Game::Flag(int xpos, int ypos)
{
	if (xpos<xleft || xpos>xright || ypos<ytop || ypos>ybottom)
		return;
	if (m_bPlaying == FALSE)
	{
		m_bPlaying = TRUE;
		t_start = clock();
	}
	int x = 1;
	int y = 1;
	for (x = 1; x < X; x++)
		if (xpos < x * 30 + xleft)
			break;
	for (y = 1; y < Y; y++)
		if (ypos < y * 30 + ytop)
			break;
	x -= 1; //배열은 0,0 부터 시작
	y -= 1;

	if (board[x][y].first <= 10) //깃발이 아니라면
	{
		if (board[x][y].second == true) //이미 밝혀진 숫자라면
		{
			cout << "Wrong Number." << endl;
			return;
		}

		board[x][y].second = true;
		board[x][y].first += 110; //깃발 심기
		Load(m_Board[x][y], L"image/sqaureflag.bmp");
		numFlag += 1;

		return;
	}
	else //깃발이라면
	{
		board[x][y].second = false;
		board[x][y].first -= 110;
		Load(m_Board[x][y], L"image/sqaure.bmp");
		numFlag -= 1;

		return;
	}
}

int Game::GetDig() const
{
	return numDig;
}

int Game::GetFlag() const
{
	return numFlag;
}

void Game::DrawAll()
{
	if (b_Mine == TRUE)
		return;

	for (int x = 0; x < X; x++)
	{
		for (int y = 0; y < Y; y++)
		{
			if (board[x][y].second == true) //VISIBLE이 TRUE라면
			{
				if (board[x][y].first == 0)
					Load(m_Board[x][y], L"image/sqaure2.bmp");
				else if (board[x][y].first <= 9) //숫자는 출력
					DrawNum(x, y);
			}
			else
			{
				Load(m_Board[x][y], L"image/sqaureflag.bmp");
			}
		}
	}

	b_Mine = TRUE;
}

void Game::DrawMine()
{
	if (b_Mine == TRUE)
		return;

	for (int x = 0; x < X; x++)
	{
		for (int y = 0; y < Y; y++)
		{
			if (board[x][y].second == true) //VISIBLE이 TRUE라면
			{
				if (board[x][y].first >= 110)
				{
					Load(m_Board[x][y], L"image/sqaurewrongflag.bmp");
				}
			}
			else if (board[x][y].first == -1) //아직 못찾은 지뢰는
				Load(m_Board[x][y], L"image/sqauremine.bmp");
		}
	}

	b_Mine = TRUE;
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::GameLoop(HDC hdc)
{
	DrawAll(hdc);
	Update();
}

void Game::DrawAll(HDC hdc)
{
	switch (m_nGameState)
	{
	case TITLE:
		DrawBG();
		DrawTitle();
		DrawMenu();
		DrawSelect();
		break;
	case GAME_READY:
		DrawBG();
		DrawTitle();
		DrawDif();
		DrawSelect();
		break;
	case GAME_EASY:
		DrawBG();
		DrawHome();
		DrawSmile();
		DrawnumFlag();
		DrawTime();
		DrawBoard();
		break;
	case GAME_NORMAL:
		DrawBG();
		DrawHome();
		DrawSmile();
		DrawnumFlag();
		DrawTime();
		DrawBoard();
		break;
	case GAME_HARD:
		DrawBG();
		DrawHome();
		DrawSmile();
		DrawnumFlag();
		DrawTime();
		DrawBoard();
		break;
	case GAME_OVER:
		DrawBG();
		DrawHome();
		DrawBoard();
		DrawDead();
		DrawMine();
		DrawnumFlag();
		DrawFinishTime();
		break;
	case GAME_CLEAR:
		DrawBG();
		DrawHome();
		DrawBoard();
		DrawCool();
		DrawAll();
		DrawnumFlag();
		DrawFinishTime();
		break;

	default:
		break;
	}

	BitBlt(hdc, 0, 0, m_Base.bit.bmWidth, m_Base.bit.bmHeight, m_Base.dc, 0, 0, SRCCOPY);
}

void Game::Update()
{
	switch (m_nGameState)
	{
	case TITLE:
		UpdateSelect();
		break;
	case GAME_READY:
		break;
	case GAME_EASY:
		canClear();
		break;
	case GAME_NORMAL:
		canClear();
		break;
	case GAME_HARD:
		canClear();
		break;
	default:
		break;
	}
}
