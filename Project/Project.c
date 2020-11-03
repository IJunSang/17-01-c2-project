/*
	2017/06/09
	���α׷� ����: Ű���� ����
	���α׷� ����: �������� ���ܾ���� ����ģ��. ���� ������ ���� ���������� �ٲ��,
	�ٲ� ���������� ���� �������� ���ܾ���� �ӵ��� ��������
	�ܾ��� ������ �� 78��, ��� �� ���߸� ���� Ŭ����, 3����� �� ������� ���� ������ �ȴ�
*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

//�ʿ��� ����ü ����
typedef struct
{
	int x;
	int y;
	char in_use;
	char word[15];
	char overlap;
}INFO;
//�Լ� ������Ÿ�� ����
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

//�ʿ��� �������� ����
INFO words[78];
int stage = 1;
int score = 0;
int count = 1;
int life = 3;

int main(int argc, char *argv[])
{
	//�����忡 ����� ���� ����
	DWORD ThreadID = 1;
	DWORD ThrdParam = 1;

	srand(time(NULL));	//������ ����
	initiate();	//�ؽ�Ʈ���Ͽ��� �ܾ� �о���� �� �ʿ��� �� �ʱ�ȭ
	printMenu();	//�޴� ���
	getche();
	//������ ���� �� �Լ� ����
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
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n"); 
	printf("����������������������������������������������\n");
	gotoxy(27, 20);
	printf("Press any key to continue...\n");
}

void initiate()
{
	int i = 0;
	//word.txt������ �б���� �ҷ���
	FILE *fp = fopen("word.txt", "r");
	if (fp == NULL)
	{
		printf("File Read Error!\n");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{//���ϳ����� �ܾ� ��� ���� �� words����ä�迭�� word���ڿ��� �������
			fscanf(fp, "%s", &words[i++].word);
		}
	}
	for (i = 0; i < 78; i++)
	{	//�ʿ��� ���� �ʱ�ȭ
		words[i].y = 1;
		words[i].in_use = 0;
		words[i].overlap = 0;
	}
	fclose(fp);
}


void printInfo()
{
	//�ϴܿ� ���, ��������, ����, �Է¹޴� �� �޴��� ������ ���� ���
	int i;
	gotoxy(1, 24);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	printf("Life : ");
	for (i = 0; i < life; i++)
		printf("��");

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
	//�ϴ� ���� �޴� ���� �� �ٽ� ���
	gotoxy(1, 24);
	printf("                                                                       ");
	printInfo();
	for (i = 0; i < 78; i++)
	{	//�ܾ ���� x�ప�� �������� ������ ����
		words[i].x = rand() % 65 + 1;
	}
	while (1)
	{
		//78���� �ܾ��� �ϳ� �������� ��� ���
		index = rand() % 78;
		if (score == 78)//txt���Ͽ� �ִ� �ܾ� 78���� ��� ������ �� ���ڿ� ��� �� ���� ����
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
		words[index].in_use = 1;	//�ܾ ����ٴ� �ǹ̷� 1�� �� ��ȯ
		words[index].overlap = 1;	//�ܾ ����ٴ� �ǹ̷� 1�� �� ��ȯ(�ٽ� ������� ���ϰ� ��)
		printNextLine();//�����ٷ� �̵�
		print();//�ֿܼ� ���� ���ڵ� ���
		for (i = 0; i < 78; i++)
		{
			if (words[i].y == 21)
			{	//�ܾ y�� 21��°���� �������� �ܾ ����� ����� �ϳ� ���ҽ�Ų��
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
			if (life == 0)	//����� 0�̸� gameOver�Լ��� ���ڿ� ��� �� ���� ����
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
		{	//words����ü�� y�ప�� �ϳ� ������Ŵ���μ� ������ ���� ���ڵ��� ��ĭ�� ������ ������
			words[i].y++;
		}
	}
	//������������ ���� ���ڰ� ���� �ð� ����
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
		if (ch == 8)//�Է¹��� ���ڰ� �齺���̽��ΰ��
		{
			count--;
			if (count < 0)//count�� 0 �Ʒ��� �������� �ٽ� 0���� ����
				count = 0;
			gotoxy(20 + count, 24);	//�齺���̽� ����� ȿ��
			printf(" ");
		}
		else if (ch == 13)//����Ű�� �Է¹޾��� ���
		{
			for (i = 0; i < 78; i++)
				if (words[i].in_use == 1)//�Է¹��� �ܾ� ��
					if (!strncmp(words[i].word, keyboard, strlen(words[i].word)))
					{	//�¾��� ��� ���� ����, �ܾ� ����� ������Ű�� ���� in_use�� 0���� ��ȯ �� ���
						score++;
						words[i].in_use = 0;
						print();
						//score���� ���� stage�� ����
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
			//�� �� �� score�� ���� ���� �ϴ� �޴� ���� �� ���
			gotoxy(1, 24);
			printf("                                                                       ");
			printInfo();
			memset(keyboard, '\0', strlen(keyboard));
		}
		else
		{	//���ڿ� �Է½� �״�� ��� �� keyboard�迭�� ����
			gotoxy(20 + count, 24);
			printf("%c", ch);
			keyboard[count++] = ch;
		}
	}

	return 0;
}

void gameOver()
{
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
}

void print()
{	//��� �ܾ� ����ϴ� �Լ�
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
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
	printf("����������������������������������������������\n");
}