
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <fstream.h>
#include "Screen.h"

void Init();
void Update();
void Render();
void Release();
void gotoxy(int x, int y);

int Force_x, Force_y;
int RoopCount = 0;
int SnakeSpeed = 200;
bool Playing = true;
int Score = 0;
bool SnakeDieColor = false;
int HighScore = 0;

bool AnyScore_visible = false;
int AnyScore_count = 0;
int AnyScore_x = 50;
int AnyScore_y = 20;

struct SnakeBody
{
public:
	int x, y;
	int Old_x, Old_y;
	SnakeBody* Prev;
	SnakeBody* Next;

	SnakeBody()
	{
		Init();
	}
	~SnakeBody()
	{

	}
	void Init()
	{
		x = 50;
		y = 20;
		Old_x = 48;
		Old_y = 20;
		Prev = NULL;
		Next = NULL;
	}
};

SnakeBody Head;
SnakeBody* Tail;

struct Item
{
public:
	int x, y, tyep;
	
	Item* Prev;
	Item* Next;

	Item()
	{
		x = 50;
		y = 20;
		tyep = 0;
		Prev = NULL;
		Next = NULL;
	}
	~Item()
	{

	}
};

struct ItemManager
{
public:
	Item* Start;
	Item* End;
	int Count;

	ItemManager()
	{
		Start = NULL;
		End = NULL;
		Count = 0;
	}
	~ItemManager()
	{
		
	}

	Item* Find()
	{
		Item* Now = Start;

		for (;;)
		{
			if (!Now)
			{
				return NULL;
			}

			if(Now->x == Head.x && Now->y == Head.y)
			{
				return Now;
			}
			
			Now = Now->Next;
		}
	}

	void Add(int x, int y, int tyep)
	{
		if(Count>=7)
		{
			return;
		}
		Item* Now = Start;

		for(;;)
		{
			if(Now == NULL)
			{
				break;
			}
			if(Now->x == x && Now->y == y)
			{
				return;
			}
			Now = Now->Next;
		}

		Item* N = new Item();

		N->x = x;
		N->y = y;
		N->tyep = tyep;
		
		if(End == NULL)
		{
			Start = N;
			End = N;
		}
		else
		{
			End->Next = N;
			N->Prev = End;
			End = N;
		}
		


		Count++;
		
		//Start = N;
	}

	void Del(Item* Ritem)
	{
		AnyScore_x = Ritem->x;
		AnyScore_y = Ritem->y;
		AnyScore_visible = true;
		AnyScore_count = 0;

		if(Ritem == Start)
		{
			if(Start == End)
			{
				Start = NULL;
				End = NULL;
			}
			else
			{
				Start = Ritem->Next;
			}
		}
		else if(Ritem == End)
		{
			End = Ritem->Prev;
			Ritem->Prev->Next = NULL;
		}
		else
		{
			Ritem->Next->Prev = Ritem->Prev;
			Ritem->Prev->Next = Ritem->Next;
		}
		delete Ritem;
		Count--;
		RoopCount = 0;

		if (SnakeSpeed >= 20)
			SnakeSpeed -= 5;

		//Beep(1000, 100);
		Score+=10;
	}
};

ItemManager itemManager;

void Map()
{
	SetColor(15); //하양색
	for (int i = 0; i < 99; i += 2)
	{
		ScreenPrint(i, 40, "■");
		ScreenPrint(i, 0, "■");
	}
	for (int i = 0; i < 40; i++)
	{
		ScreenPrint(0, i, "■");
		ScreenPrint(98, i, "■");
	}
}

void Makeitem()
{
	int Item_ran_x = 0;
	int Item_ran_y = 0;

	srand(time(NULL));

	Item_ran_x = ((rand() % 95) / 2 * 2) + 2;
	Item_ran_y = (rand() % 38) + 1;

	itemManager.Add(Item_ran_x, Item_ran_y, 0);

}

void Input()
{
	
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Force_x = -2;
		Force_y = 0;
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Force_x = 2;
		Force_y = 0;
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		Force_x = 0;
		Force_y = -1;
	}
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		Force_x = 0;
		Force_y = 1;
	}
}

void Title()
{
	ScreenPrint(43, 10, "스네이크 게임");

	ScreenPrint(36, 15, "시작하려면 아무키나 누르세요");

	ScreenFlipping();

	getch();
}


void OutputPlay()
{
	SnakeBody* Now = &Head;
	for(;;)
	{
		if(!Now)
		{
			break;
		}
		if (Now == &Head || SnakeDieColor == true)
			SetColor(12); // 빨강색
		else
			SetColor(10); // 하양색
		ScreenPrint(Now->x, Now->y, "●");
		Now = Now->Next;
	}

	Item* itemNow = itemManager.Start;
	for (;;)
	{
		if (!itemNow)
		{
			break;
		}
		SetColor(14); // 노랑색
		ScreenPrint(itemNow->x, itemNow->y, "★");
		itemNow = itemNow->Next;
	}

	SetColor(15); // 하양색
	
	char string[100];
	sprintf_s(string, "Game Score = %d ..... Speed = %d ..... Move = ↑ ↓ → ←",Score,SnakeSpeed );
	ScreenPrint(4,42, string );

	if(AnyScore_visible)
	{
		//if(RoopCount%2==0)
		{
			AnyScore_count++;
			if(AnyScore_count == 4)
			{
				AnyScore_visible = false;
			}
		}
		SetColor(11);
		ScreenPrint(AnyScore_x, AnyScore_y-AnyScore_count, "+10");
	}
}

void OutputGameOver()
{
	char string[100];

	SetScore();
	
	SetColor(14);
	sprintf_s(string, "High Score★ : %d", HighScore);
	ScreenPrint(42, 12, string);
	SetColor(15);

	ScreenPrint(44, 19, "Game Over!!");

	
	sprintf_s(string, "Game Score : %d", Score);
	ScreenPrint(42, 23, string);

	ScreenPrint(42, 25, "다시 시작: Space");

	if(kbhit())
	{
		if(getch() == 32) //초기화
		{
			Playing = true;
			SnakeDieColor = false;
			itemManager.Start = NULL;
			itemManager.End = NULL;
			itemManager.Count = 0;
			RoopCount = 0;

			Head.Init();
			Tail = &Head;
			Force_x = 2;
			Force_y = 0;
			Score = 0;
			SnakeSpeed = 200;
		}
	}
}

void NewBody()
{
	SnakeBody* N = new SnakeBody();
	Tail->Next = N;
	N->Prev = Tail;
	N->Old_x = N->x = Tail->Old_x;
	N->Old_y = N->y = Tail->Old_y;
	Tail = N;
}

bool CheckBody()
{
	SnakeBody* Now = Head.Next;
	for (;;)
	{
		if (!Now)
		{
			break;
		}
		if (Now->x == Head.x && Now->y == Head.y)
			return false;

		Now = Now->Next;
	}

	return true;
}

bool CheckLine()
{
	if (Head.x < 1 || Head.x > 96 || Head.y < 1 || Head.y > 39)
		return false;
	
	return true;
}
bool Checkitem()
{
	Item* Ritem = itemManager.Find();

	if (Ritem == NULL)
	{
		return false;
	}
	else 
	{
		itemManager.Del(Ritem);
		return true;
	}
		
}

void SetScore()
{
	ifstream fin("score.txt");

	fin.read((char*)&HighScore, sizeof(int));

	if (HighScore < Score)
	{
		ofstream fout("score.txt");

		HighScore = Score;

		fout.write((char*)&HighScore, sizeof(int));

		fout.close();
	}

	fin.close();
}

int main()
{
	Init();
	for(;;)
	{
		if(Playing)
		{
			RoopCount++;
			Update();
			if (Checkitem())
			{
				NewBody();
			}
			if (RoopCount % 10 == 0)
			{
				Makeitem();
			}
		}
		Render();
		if (!CheckBody() || !CheckLine())
		{
			SnakeDieColor = true;
			for (;;)
			{
				if (!kbhit())
				{
					break;
				}
				getch();
			}
			Render();
			Playing = false;
			Sleep(500);
		}
			Sleep(SnakeSpeed); // 뱀 속도
	}

	Release();

	return 0;
}

void BackUp()
{
	SnakeBody* Now = &Head;

	for (;;)
	{
		if (!Now)
		{
			break;
		}
		Now->Old_x = Now->x;
		Now->Old_y = Now->y;

		Now = Now->Next;
	}
}
void Init()
{
	Tail = &Head;
	
	ScreenInit();
	Force_x = 2;
	Force_y = 0;

	Title();
}

void Update()
{
	Input();
	
	BackUp();

	Head.x += Force_x;
	Head.y += Force_y;

	SnakeBody* Now = Head.Next;

	for (;;)
	{
		if (!Now)
		{
			break;
		}
		Now->x = Now->Prev->Old_x;
		Now->y = Now->Prev->Old_y;

		Now = Now->Next;
	}
}

void Render()
{
	ScreenClear();

	Map();

	if (Playing)
		OutputPlay();
	else
		OutputGameOver();

	ScreenFlipping();
}

void Release()
{
	ScreenRelease();

}

