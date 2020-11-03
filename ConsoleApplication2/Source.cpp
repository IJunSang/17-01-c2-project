#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <conio.h>


#define LEVEL 20
#define QUESTION 50

typedef struct _Word {
	unsigned int id;
	char word[64];
	POINT loc;
	bool be;
} Word;

void SetColor(int color);
void Cursorgotoxy(int x, int y);
void CreateWord(void);
void MoveWord(int Speed);
void Compare(char* _str);
UINT WINAPI Input(void *arg);
void LevelSpeed(void);
void SetStage(void);

unsigned int count = 0;
int inc = 0;
Word Qus[50];
bool status = FALSE;
int point = 0;
int level = 1;
DWORD ThreadID;
HANDLE Thread;

int lv[LEVEL] = { 0, 3, 7, 11, 16, 20, 25, 30, 40, 50, 60, 80, 100, 120, 140, 160, 190, 210, 240, 270 };
int pt[23] = { 1, 20, 40, 10, 30, 10, 30, 50, 20, 1, 50, 1, 60, 40, 10, 50, 20, 1, 60, 10, 1, 40, 20 };
const char Prob[QUESTION][32] = {
	{ "Hello" },
	{ "Turtle" },
	{ "Human" },
	{ "Computer" },
	{ "Headset" },
	{ "School" },
	{ "Paper" },
	{ "Aion" },
	{ "English" },
	{ "Monitor" },
	{ "Program" },
	{ "Monster" },
	{ "Money" },
	{ "Book" },
	{ "Metro" },
	{ "Speaker" },
	{ "Cisco" },
	{ "Network" },
	{ "Internet" },
	{ "Comic" },
	{ "Box" },
	{ "Dragon" },
	{ "Ten" },
	{ "Boss" },
	{ "My" },
	{ "Mirror" },
	{ "Java" },
	{ "Ham" },
	{ "Forder" },
	{ "Directory" },
	{ "Tear" },
	{ "Inject" },
	{ "Metrics" },
	{ "Coin" },
	{ "Five" },
	{ "Seven" },
	{ "Moon" },
	{ "Skull" },
	{ "Cylinder" },
	{ "Magic" },
	{ "Warrior" },
	{ "Every" },
	{ "Game" },
	{ "Pointer" },
	{ "Lazer" },
	{ "Sport" },
	{ "Jump" },
	{ "Boat" },
	{ "Pig" },
	{ "Duck" },
};


enum _ColorSet
{
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKY_BLUE,
	DARK_RED,
	DARK_VIOLET,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKY_BLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

int main(void)
{
	memset(&Qus, 0, sizeof(Word));
	srand((unsigned int)time(NULL));

	Thread = (HANDLE)_beginthreadex(NULL, 0, Input, (void*)NULL,
		0, (unsigned*)&ThreadID);
	SetColor(WHITE);

	SetStage();

	while (1)
	{
		CreateWord();
		LevelSpeed();
	}

	return 0;
}
void Compare(char* _str)
{
	for (int i = 0; i<50; i++)
	{
		if (Qus[i].be == TRUE)
		{
			if (!strcmp(_str, Qus[i].word))
			{
				point++;
				for (int j = 0; j<LEVEL; j++)
				{
					if (lv[j] <= point && lv[j + 1] > point)
					{
						level = j + 1;
						break;
					}
				}
				Cursorgotoxy(Qus[i].loc.x, Qus[i].loc.y);
				printf("           ");
				Cursorgotoxy(50, 24);
				printf("점수: ");
				SetColor(SKY_BLUE);
				printf("%d   ", point);
				SetColor(WHITE);
				Cursorgotoxy(64, 24);
				printf("레벨: ");
				SetColor(SKY_BLUE);
				printf("%d   ", level);
				SetColor(WHITE);
				Cursorgotoxy(20 + inc, 24);

				memset(&Qus[i], 0, sizeof(Word));
				break;
			}
		}
	}
}
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void Cursorgotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void MoveWord(int Speed)
{
	for (int i = 0; i<50; i++)
	{
		if (Qus[i].be == TRUE)
		{
			Cursorgotoxy(Qus[i].loc.x, Qus[i].loc.y);
			printf("           ");
			Qus[i].loc.y += Speed;
			if (Qus[i].loc.y > 21)
			{
				memset(&Qus[i], 0, sizeof(Word));
				continue;
			}
			Cursorgotoxy(Qus[i].loc.x, Qus[i].loc.y);
			printf("%s", Qus[i].word);
			Cursorgotoxy(20 + inc, 24);
		}
	}
}
void CreateWord(void)
{
	static int rem = 0;
	int index;

	index = rand() % QUESTION;
	for (int i = 0; i<50; i++)
	{
		if (Qus[i].be == FALSE)
		{
			Qus[i].id = count++;
			strcpy(Qus[i].word, Prob[index]);
			Qus[i].loc.x = pt[rem++];
			Qus[i].loc.y = 1;
			if (rem > 22)
				rem = 0;
			Qus[i].be = TRUE;

			MoveWord(1);

			Cursorgotoxy(Qus[i].loc.x, Qus[i].loc.y);

			printf("%s", Qus[i].word);
			Cursorgotoxy(20 + inc, 24);

			break;
		}
	}
}
UINT WINAPI Input(void *arg)
{
	char str[32] = { 0 };

	while (1)
	{
		int key = getch();
		if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') && inc < 20)
		{
			Cursorgotoxy(20 + inc, 24);
			SetColor(YELLOW);
			printf("%c", key);
			SetColor(WHITE);
			str[inc++] = key;
			str[inc] = '\0';
			if (inc > 20)
				inc = 20;
		}
		else if (key == 8)
		{
			if (inc > 0)
			{
				str[inc--] = key;
				str[inc] = '\0';
				Cursorgotoxy(20 + inc, 24);
				printf(" ");
			}
		}
		else if (key == 13)
		{
			Compare(str);
			inc = 0;
			Cursorgotoxy(20, 24);
			printf("                      ");
			Cursorgotoxy(20, 24);
		}
	}
	return 0;
}
void LevelSpeed(void)
{
	Sleep(2500 - level * 125);
}
void SetStage(void)
{
	Cursorgotoxy(14, 24);
	printf("입력: ");
	SetColor(GREEN);
	Cursorgotoxy(1, 23);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
	Cursorgotoxy(50, 24);
	SetColor(WHITE);
	printf("점수: ");
	SetColor(SKY_BLUE);
	printf("%d   ", point);
	SetColor(WHITE);
	printf("    레벨: ");
	SetColor(SKY_BLUE);
	printf("%d   ", level);
	SetColor(WHITE);
}