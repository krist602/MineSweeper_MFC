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
	Load(m_Smile, L"image/smile.bmp");
	for(int i = 0; i<32; i++)
		for (int j = 0; j < 18; j++)
			Load(m_Board[i][j], L"image/sqaure.bmp");
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

	for (int i = 0; i < X; i++)
		for (int j = 0; j < Y; j++)
			board[j][i] = { 0, false };
	
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
		if (board[y][x].first == -1)
			continue;
		else
			board[y][x].first = -1;
		//cout << i << "��° ���� : " << y << " " << x << endl;

		//���ڸ� ���� ���Ŀ� ���� ������ ���ڵ��� +=1�� ���ش�.
		if (x == 0)
		{
			if (y == 0)
			{
				for (int j = x; j <= x + 1; j++)
				{
					for (int k = y; k <= y + 1; k++)
					{
						if (board[k][j].first == -1)
							continue;
						else
							board[k][j].first += 1;
					}
				}
			}
			else
			{
				for (int j = x; j <= x + 1; j++)
				{
					for (int k = y - 1; k <= y + 1; k++)
					{
						if (board[k][j].first == -1)
							continue;
						else
							board[k][j].first += 1;
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
					if (board[k][j].first == -1)
						continue;
					else
						board[k][j].first += 1;
				}
			}
		}
		else
		{
			for (int j = x - 1; j <= x + 1; j++)
			{
				for (int k = y - 1; k <= y + 1; k++)
				{
					if (board[k][j].first == -1)
						continue;
					else
						board[k][j].first += 1;
				}
			}
		}
		i++;
	}
}

void Game::DrawBG()
{
	StretchBlt(m_Base.dc, 0, 0, XRES+150, YRES, m_BG.dc, 0, 0, XRES+150, YRES, SRCCOPY);
	/*BitBlt(m_Base.dc,
		0, 0,
		m_BG.bit.bmWidth,
		m_BG.bit.bmHeight,
		m_BG.dc,
		0, 0,
		SRCCOPY
	);*/
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
		InitLevel();
		break;
	case 1:
		m_nGameState = GAME_NORMAL;
		InitLevel();
		break;
	case 2:
		m_nGameState = GAME_HARD;
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
		MessageBox(NULL, name, L"��Ʈ�� �ε� ����", MB_OK);
		exit(0);
	}
	h.dc = CreateCompatibleDC(m_Base.dc);
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);
	GetObject(h.hbmp, sizeof(BITMAP), &h.bit);

}

RECT& Game::GetRect()
{
	// TODO: ���⿡ return ���� �����մϴ�.
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
	int y = m_rtClient.bottom/2 - Y * 15 - 60;

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
}

void Game::DrawDead()
{
	Load(m_Smile, L"image/dead.bmp");
}

void Game::DrawCool()
{
	Load(m_Smile, L"image/cool.bmp");
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
	x -= 1; //�迭�� 0,0 ���� ����
	y -= 1;
	if (board[x][y].second == true)
		return;
	if (board[x][y].first <= 10)
	{
		if (board[x][y].first == -1) //-1�̶��
		{
			//���ڰ� ������.
			Load(m_Board[x][y], L"image/sqaurebombmine.bmp");
			board[x][y].first = -10;
			DrawDead();
			DrawMine();
			return;
		}
		else if (board[x][y].first == 0) //������ ���ڰ� ���ٸ�
		{
			if (board[x][y].second == false)
				numDig -= 1;
			board[x][y].second = true; // ������ 0���� �����
			Load(m_Board[x][y], L"image/sqaure2.bmp");
			for (int j = xpos - 30; j <= xpos + 30; j=j+30)
			{
				for (int k = ypos - 30; k <= ypos + 30; k=k+30)
				{
					Dig(j, k); //������ �ٽ� Ž��
				}
			}
		}
		else //
		{
			board[x][y].second = true;
			DrawNum(x, y);
			//Load(m_Board[x][y], L"image/sqaure2.bmp");
			numDig -= 1;
			return;
		}
	}
	else if (board[x][y].first >= 100)
	{
		return;
	}
	else
	{
		//cout << "���� ��� ���� ��� ���Ծ�" << endl;
		return;
	}
	return;
}

void Game::Flag(int xpos, int ypos)
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
	x -= 1; //�迭�� 0,0 ���� ����
	y -= 1;

	if (board[x][y].first <= 10) //����� �ƴ϶��
	{
		if (board[x][y].second == true) //�̹� ������ ���ڶ��
		{
			cout << "Wrong Number." << endl;
			return;
		}

		board[x][y].second = true;
		board[x][y].first += 110; //��� �ɱ�
		Load(m_Board[x][y], L"image/sqaureflag.bmp");
		numFlag += 1;

		return;
	}
	else //����̶��
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
		for (int x = 0; x <= X; x++)
	{
		for (int y = 0; y <= Y; y++)
		{
			//cout << board[j][i].first << " ";
			if (board[x][y].second == true) //VISIBLE�� TRUE���
			{
				if (board[x][y].first == 0)
					Load(m_Board[x][y], L"image/sqaure2.bmp");
				else if (board[x][y].first <= 9) //���ڴ� ���
					DrawNum(x, y);
				else if (board[x][y].first > 100)//����� *�� ǥ��
					Load(m_Board[x][y], L"image/sqaureflag.bmp");
			}
			else
			{
				cout << "�� ";
			}
		}
		cout << endl;
	}
}

void Game::DrawMine()
{
	for (int x = 0; x <= X; x++)
	{
		for (int y = 0; y <= Y; y++)
		{
			if (board[x][y].first == -1) //���� ��ã�� ���ڴ�
			{
				Load(m_Board[x][y], L"image/sqauremine.bmp");
				continue;
			}
			//if (board[x][y].first < -5) //��Ƽ� ���� ���ڴ�
			//{
			//	cout << "�� ";
			//	continue;
			//}

			if (board[x][y].second == true) //VISIBLE�� TRUE���
			{
				if (board[x][y].first == 0) // 0�� ��������.
				{
					cout << "   ";
					continue;
				}
				else if (board[x][y].first <= 9) //���ڴ� ���
				{
					cout << " " << board[x][y].first << " ";
					continue;
				}
				else if (board[x][y].first <= 100)
				{
					cout << board[x][y].first << " ";
					continue;
				}
				else if (board[x][y].first > 100)//����� *�� ǥ��
				{
					if (board[x][y].first - 110 == -1)
						cout << "�� ";
					else if (board[x][y].first - 110 >= 0)
						Load(m_Board[x][y], L"image/sqaurewrongflag.bmp");
				}
			}
			else
			{
				cout << "�� ";
			}
		}
		cout << endl;
	}
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
		DrawSmile();
		DrawBoard();
		break;
	case GAME_NORMAL:
		DrawBG();
		DrawSmile();
		DrawBoard();
		break;
	case GAME_HARD:
		DrawBG();
		DrawSmile();
		DrawBoard();
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
		break;
	case GAME_NORMAL:
		break;
	case GAME_HARD:
		break;
	default:
		break;
	}
}