#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "game.h"

#define MAX 40
#define BACKCOLOR 6 // white

struct ST_OBJECT
{
	int x = 0;
	int y = 0;
	bool bActive = false;
};


ST_OBJECT g_stEnemy[MAX];
ST_OBJECT g_stPlayer;
ST_OBJECT stMissile;

void Spawn(void)//별 생성 함수
{

	for (int i = 0; i < MAX; i++)
	{
		if (!g_stEnemy[i].bActive)
		{
			g_stEnemy[i].x = (rand() % 20) * 2;
			g_stEnemy[i].y = 0;
			g_stEnemy[i].bActive = true;
			break;
		}
	}
}

void InputProcess()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		g_stPlayer.x-=2;
		if (g_stPlayer.x < 0) g_stPlayer.x = 0;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		g_stPlayer.x+=2;
		if (g_stPlayer.x > 40) g_stPlayer.x = 40;
	}
#ifdef ACTIVE_MISSILE
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (!stMissile.mActive)
		{
			stMissile.x = g_stPlayer.x;
			stMissile.y = g_stPlayer.y - 1;
			stMissile.mActive = true;
		}
	}
#endif
}

int EnemyProcess()
{
	int count = 0;
	for (int i = 0; i < MAX; i++)
	{
		if (g_stEnemy[i].bActive)
		{
			SetColor(BACKCOLOR, 0);
			count++;
			GotoXY(g_stEnemy[i].x, g_stEnemy[i].y);
			printf("☆");
			g_stEnemy[i].y++;

			// 충돌 시,
			if (g_stEnemy[i].y == g_stPlayer.y) 
			{
				if (g_stEnemy[i].x == g_stPlayer.x)
					g_stPlayer.bActive = false;
			}
			
			// 별이 내려오다가 화면 밑 끝에 다다를 때,
			if (g_stEnemy[i].y > g_stPlayer.y)
			{
				g_stEnemy[i].bActive = false;
			}
		}
	}
	return count;
}

void RenderPlayer()
{
	SetColor(BACKCOLOR, 12);
	if (g_stPlayer.bActive)
	{
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("▲");
	}
	else//충돌 됨
	{
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("▒");
	}
}

void Init()
{
	g_stPlayer.x = 14;
	g_stPlayer.y = 28;
	g_stPlayer.bActive = true;

	srand(time(NULL));
	SetConsoleSize(40, 30);

	//SetTitle("별피하기");
	SetColor(BACKCOLOR, 0);

}

void Missile()
{
	if (stMissile.bActive)
	{
		GotoXY(stMissile.x, stMissile.y);
		printf("↑");
		stMissile.y--;
		if (stMissile.y < 0) stMissile.bActive = false;
	}
}

int g_iScore = 0;
void Score()
{
	if(g_stPlayer.bActive)	g_iScore++;
	GotoXY(0, 0);
	printf("Score : %d", g_iScore);
}


void StartMenu(void)
{
	while (_kbhit()) _getch();

	while (1)
	{
		Clear();

		GotoXY(10, 9);
		printf("별 피하기");
		GotoXY(10, 20);
		printf("Press Any key ...");

		if (_kbhit()) break;
		Sleep(40);
	}
}

void GameMain() 
{
	g_stPlayer.x = 14;
	g_stPlayer.y = 28;
	g_stPlayer.bActive = true;
	g_iScore = 0;

	int state = 0;
	while (1)
	{
		Clear();

		//별이 비행기에 닿지 않았을 때
		if (g_stPlayer.bActive)
		{
			//별 생성 함수
			Spawn();

			// input 처리
			InputProcess();
		}

		//미사일 처리
		//Missile();

		//별 처리
		state = EnemyProcess();

		//Player 출력
		RenderPlayer();
		Score();
		Sleep(50);

		if (state == 0) break;
	}
}

bool ResultMenu()
{
	bool bRet = false;
	while (1)
	{
		Clear();
		GotoXY(10, 9);
		printf("Result Menu");
		GotoXY(10, 15);
		printf("SCORE : %d", g_iScore);

		GotoXY(0, 24);
		printf("continue ... Y/N");
		if (GetAsyncKeyState('Y') & 0x8000)
		{
			bRet = false;
			break;
		}
		if (GetAsyncKeyState('N') & 0x8000)
		{
			bRet = true;
			break;
		}
		Sleep(40);
	}
	return bRet;
}

int main()
{
	
	//Init Player and Console
	Init();

	while (1) 
	{
		StartMenu();
		GameMain();
		if (ResultMenu()) break;
	}

	system("pause");
	return 0;
}