/*
	2017/06/09
	프로그램 제목: 키보드 게임
	프로그램 설명: 내려오는 영단어들을 따라친다. 맞은 갯수에 따라 스테이지가 바뀌고,
	바뀐 스테이지에 따라 내려오는 영단어들의 속도가 빨라진다
	단어의 갯수는 총 78개, 모두 다 맞추면 게임 클리어, 3목숨이 다 사라지면 게임 오버가 된다
*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

//필요한 구조체 선언
typedef struct
{
	int x;
	int y;
	char in_use;
	char word[15];
	char overlap;
}INFO;
//함수 프로토타입 선언
void gotoxy(int x, int y);
void printMenu();
void initiate();
void printInfo();
void printWords();
void printNextLine();
void gameOver();
void print();
void gameClear();
DWORD WINAPI input(LPVOID lpParam);

//필요한 전역변수 선언
INFO words[78];
int stage = 1;
int score = 0;
int count = 1;
int life = 3;

int main(int argc, char *argv[])
{
	//쓰레드에 사용할 변수 생성
	DWORD ThreadID = 1;
	DWORD ThrdParam = 1;

	srand(time(NULL));	//난수값 생성
	initiate();	//텍스트파일에서 단어 읽어오기 및 필요한 값 초기화
	printMenu();	//메뉴 출력
	getche();
	//쓰레드 생성 및 함수 실행
	HANDLE hThread = CreateThread(NULL, 0, input, &ThrdParam, 0, &ThreadID);
	system("cls");
	while (1)
	{
		printWords();
	}

	return 0;
}

void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void printMenu()
{
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	printf("□■□□■□■■■■□■□□□■□■■■□□□■■■■□□□□■□□□■■■□□■■■□□\n");
	printf("□■□■□□■□□□□□■□■□□■□□■□■■□□■■□□■□■□□■□□■□■□□■□\n");
	printf("□■■□□□■■■■□□□■□□□■■■□□■□□□□■□■■■■■□■■■□□■□□■□\n");
	printf("□■□■□□■□□□□□□■□□□■□□■□■■□□■■□■□□□■□■□□■□■□□■□\n");
	printf("□■□□■□■■■■□□□■□□□■■■□□□■■■■□□■□□□■□■□□■□■■■□□\n");
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■■□□□■□□□■□□□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□□□□■□■□□■■□■■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□■■□■■■■■□■□■□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□■□■□□□■□■□□□■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■□□■□□□■□■□□□■□■■■■□□□□□□□□□□□□□\n"); 
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	gotoxy(27, 20);
	printf("Press any key to continue...\n");
}

void initiate()
{
	int i = 0;
	//word.txt파일을 읽기모드로 불러옴
	FILE *fp = fopen("word.txt", "r");
	if (fp == NULL)
	{
		printf("File Read Error!\n");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{//파일끝까지 단어 모두 읽은 뒤 words구조채배열의 word문자열에 집어넣음
			fscanf(fp, "%s", &words[i++].word);
		}
	}
	for (i = 0; i < 78; i++)
	{	//필요한 값들 초기화
		words[i].y = 1;
		words[i].in_use = 0;
		words[i].overlap = 0;
	}
	fclose(fp);
}


void printInfo()
{
	//하단에 목숨, 스테이지, 점수, 입력받는 값 메뉴를 색깔을 입혀 출력
	int i;
	gotoxy(1, 24);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	printf("Life : ");
	for (i = 0; i < life; i++)
		printf("♥");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	gotoxy(14, 24);
	printf("Input: ");
	
	gotoxy(50, 24);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	printf("Score: ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
	printf("%d   ", score);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("    stage: ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	printf("%d   ", stage);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void printWords()
{
	int i, index;
	//하단 정보 메뉴 지운 뒤 다시 출력
	gotoxy(1, 24);
	printf("                                                                       ");
	printInfo();
	for (i = 0; i < 78; i++)
	{	//단어가 나올 x축값을 랜덤값을 생성해 저장
		words[i].x = rand() % 65 + 1;
	}
	while (1)
	{
		//78개의 단어중 하나 무작위로 골라서 출력
		index = rand() % 78;
		if (score == 78)//txt파일에 있는 단어 78개를 모두 맞췄을 시 문자열 출력 및 게임 종료
		{
			system("cls");
			gameClear();
			exit(0);
		}
		if (words[index].overlap == 1)
			continue;
		gotoxy(words[index].x, words[index].y);
		printf("%s", words[index].word);
		words[index].y++;
		words[index].in_use = 1;	//단어가 사용됬다는 의미로 1로 값 변환
		words[index].overlap = 1;	//단어가 사용됬다는 의미로 1로 값 변환(다시 출력하지 못하게 함)
		printNextLine();//다음줄로 이동
		print();//콘솔에 나온 문자들 출력
		for (i = 0; i < 78; i++)
		{
			if (words[i].y == 21)
			{	//단어가 y축 21번째까지 내려오면 단어를 지우고 목숨을 하나 감소시킨다
				if (words[i].in_use != 0)
				{
					words[i].in_use = 0;
					print();
					life--;
					gotoxy(1, 24);
					printf("                                                                        ");
					printInfo();
				}
			}
			if (life == 0)	//목숨이 0이면 gameOver함수로 문자열 출력 및 게임 종료
			{
				system("cls");
				gameOver();
				printf("press any key to exit....");
				getche();

				exit(0);
			}
		}
	}
}

void printNextLine()
{
	int i;
	for (i = 0; i < 78; i++)
	{
		if (words[i].in_use == 1)
		{	//words구조체의 y축값을 하나 증가시킴으로서 기존에 출력됬던 문자들을 한칸씩 밑으로 내린다
			words[i].y++;
		}
	}
	//스테이지마다 다음 문자가 나올 시간 조정
	Sleep(3000 - (500 * (stage)));
}

DWORD WINAPI input(LPVOID lpParam)
{
	int i, j, count = 0;
	char ch, keyboard[15];
	fflush(stdin);
	while (1)
	{
		ch = getch();
		if (ch == 8)//입력받은 문자가 백스페이스인경우
		{
			count--;
			if (count < 0)//count가 0 아래로 내려가도 다시 0으로 고정
				count = 0;
			gotoxy(20 + count, 24);	//백스페이스 지우기 효과
			printf(" ");
		}
		else if (ch == 13)//엔터키를 입력받앗을 경우
		{
			for (i = 0; i < 78; i++)
				if (words[i].in_use == 1)//입력받은 단어 비교
					if (!strncmp(words[i].word, keyboard, strlen(words[i].word)))
					{	//맞았을 경우 점수 증가, 단어 출력을 금지시키기 위해 in_use값 0으로 변환 및 출력
						score++;
						words[i].in_use = 0;
						print();
						//score값에 따라 stage값 조절
						if (score < 5)
							stage = 1;
						else if (score < 10)
							stage = 2;
						else if (score < 16)
							stage = 3;
						else if (score < 23)
							stage = 4;
					}
			gotoxy(20, 24);
			for (i = 0; i < count; i++)
				printf(" ");
			gotoxy(20, 24);
			count = 0;
			//값 비교 뒤 score값 갱신 위해 하단 메뉴 지운 뒤 출력
			gotoxy(1, 24);
			printf("                                                                       ");
			printInfo();
			memset(keyboard, '\0', strlen(keyboard));
		}
		else
		{	//문자열 입력시 그대로 출력 후 keyboard배열에 저장
			gotoxy(20 + count, 24);
			printf("%c", ch);
			keyboard[count++] = ch;
		}
	}

	return 0;
}

void gameOver()
{
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■■□□□■□□□■□□□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□□□□■□■□□■■□■■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□■■□■■■■■□■□■□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□■□■□□□■□■□□□■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■□□■□□□■□■□□□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□■■■■□□■□□□□□□□■□■■■■■□■■■■□□□□□□□□□□\n");
	printf("□□□□□□□□■■□□■■□□■□□□□□■□□■■□□□□■□□□■□□□□□□□□□\n");
	printf("□□□□□□□□■□□□□■□□□■□□□■□□□■■■■■□■■■■□□□□□□□□□□\n");
	printf("□□□□□□□□■■□□■■□□□□■□■□□□□■■□□□□■□□□■□□□□□□□□□\n");
	printf("□□□□□□□□□■■■■□□□□□□■□□□□□■■■■■□■□□□□■□□□□□□□□\n");
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
}

void print()
{	//상단 단어 출력하는 함수
	int i;
	for (i = 1; i <= 23; i++)
	{
		gotoxy(1, i);
		printf("                                                                        ");
	}
	for (i = 0; i < 78; i++)
	{
		if (words[i].in_use == 1)
		{
			gotoxy(words[i].x, words[i].y);
			printf("%s", words[i].word);
		}
	}
}

void gameClear()
{
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■■□□□■□□□■□□□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□□□□■□■□□■■□■■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□■■□■■■■■□■□■□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□■□□□■□■□□□■□■□□□■□■□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□■■■□□■□□□■□■□□□■□■■■■□□□□□□□□□□□□□\n");
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
	printf("□□□□□□□□■■■■□■■□□□□■■■■■□□□■□□□■■■■■□□□□□□□□□\n");
	printf("□□□□□□□■□□□□□■■□□□□■□□□□□□■□■□□■■□□■■□□□□□□□□\n");
	printf("□□□□□□□■□□□□□■■□□□□■■■■■□■■■■■□■■■■■□□□□□□□□□\n");
	printf("□□□□□□□■□□□□□■■□□□□■□□□□□■□□□■□■■□□■■□□□□□□□□\n");
	printf("□□□□□□□□■■■■□■■■■■□■■■■■□■□□□■□■■□□■■□□□□□□□□\n");
}