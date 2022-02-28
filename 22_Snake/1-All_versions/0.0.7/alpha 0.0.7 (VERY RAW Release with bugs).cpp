
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

const unsigned short widthX = 40, heightY = 20;

// �������, ���������� �� ����
char field[heightY + 2][widthX + 3]; // field[y(������)][x()]
unsigned short snakeX[200], snakeY[200]; // ���������� ����

// �������:
// ������ ��������� �������� ���� � �. �.
// void Clock(int tm);
void Settings(bool &walls, bool &SCrossing, short &key); // ���� ��������� �������������
void InputSettings(bool &Switch, short &key); // ���� ��������
void Setup(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned long &score, const unsigned short &len,
		   const unsigned short &applec,
		   const char &SumbolField, const char &SumbolWall,
		   const char &SumbolSnakeHead, const char &SumbolSnakeTrunk); // ��������� ������� ����
void ShuttingDown(); // ���������� ���� (�������� ��� ����� ����?)

// ������� ����
void Draw(const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec); // ������ ���� �������� �������� � ������� �������������� ����������

void Input(short &_x, short &_y, bool &GameOver); // ���������� (����) ����� ���������

void GenerationObjects(const unsigned short &applec, unsigned short &OldApplec,
					   unsigned short &appleX, unsigned short &appleY,
					   const char &SumbolField, const char &SumbolApple); // ��������� ��������� (������, $, ��������� ������ � ����)

void Move(unsigned short *snakeX, unsigned short *snakeY, const short &_x, const short &_y,
		  const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec, const char &SumbolField,
		  const char &SumbolSnakeHead, const char &SumbolSnakeTrunk); // ����������� �������� ������� �� ���������� ������� �������

void Logic(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned short &appleX, const unsigned short &appleY,
		   const bool &walls, const bool &SCrossing, bool &GameOver,
		   unsigned long &score, unsigned short &len, unsigned short &applec,
		   const char &SumbolWall, const char &SumbolSnakeHead); // ���������� ������� �������� �� ������������ ����� �������� � �����, �������� ����-����

void WinFunc(); // ���������� � ���������� ����������

int main()
{
	bool GameOver = FALSE; // ��������� ����
	short key = 80; // ����������� ���� (����-���), ����� (��������� ��������� � ��������� �������)

	// ������������ ����������
	short _x = 0, _y = 1; // ������������ ���� �� ����������� ���������� ����� �� ��������� �������� �����
	unsigned short appleX, appleY; // ���������� ������

	// ����������-��������
	unsigned short len = 2; // ����� ����
	unsigned long score = 0; // ���������
	unsigned short applec = 0; // ���� �����
	unsigned short OldApplec = 1; // OldApplec = applec - 1 (��� ��������� ������ ������)
	// unsigned long iteration = 0; // ���������� �������� �� ���� ����
	int GameTime; // ������� ����� ����
				  //int time; // ������� ���-�� �������`�� � ���� ($)

	// Char-����������, ����������� ��� ����������� �������� ������ �������� ���������� �� �����
	const char SumbolField = ' '; // ������ ������ ������ ����
	const char SumbolWall = '#'; // ������ �����
	const char SumbolSnakeHead = '0'; // ������ ������� ������
	const char SumbolSnakeTrunk = 'o'; // ������ �������� ��������
	const char SumbolApple = '@'; // ������ ������
	const char SumbolAppleInside = 'O'; // ������ ������������� ������

										// Settings ����������
	bool walls; // ������� ����
	bool SCrossing; // 1 - ������, 0 - �������� ������

	do
	{

		Settings(walls, SCrossing, key);
		Setup(snakeX, snakeY, score, len, applec, SumbolField,
			  SumbolWall, SumbolSnakeHead, SumbolSnakeTrunk);
		key = 80;

		do
		{
			//iteration++; // Debug ����������

			Draw(score, len, applec); // ����� ������������ ��������� ������
			Input(_x, _y, GameOver);
			GenerationObjects(applec, OldApplec, appleX, appleY,
							  SumbolField, SumbolApple);
			Move(snakeX, snakeY, _x, _y, score, len, applec, SumbolField,
				 SumbolSnakeHead, SumbolSnakeTrunk);
			Logic(snakeX, snakeY, appleX, appleY, walls, SCrossing,
				  GameOver, score, len, applec, SumbolWall, SumbolSnakeHead);

			Sleep(80); // ����� � ���������� ��������� �������� ��� �� ��������� ������� (3 ������)
		} while ((!GameOver) && (key != 27));

		if (GameOver) // ����� ������� ��������� �������
		{
			bool Switch = TRUE; // ����������

			do
			{
				system("cls");
				cout << "Do you want to play again? ";

				if (Switch) cout << "YES no\n";
				else cout << "yes NO\n";

				cout << "\nFor switch press <-- -->\n";
				cout << "For continue press \"ENTER\"\n\n";

				InputSettings(Switch, key);
			} while (key != 13);

			GameOver = !Switch;
			system("cls");
		}
	} while ((!GameOver) && (key != 27));

	ShuttingDown();

	return 0;
}


void Settings(bool &walls, bool &SCrossing, short &key)
{
	bool Switch = TRUE; // ���������� �������������

	// ������� ������������� ������� ��� ������������, ��� ����� ��������
	// ������ cout << "��������� �� ������ ��� (������?) "
	// � InputSettings(walls(��������� ��� (������?))).

	do
	{
		cout << "Settings:\n";
		cout << "Walls? ";

		if (Switch) cout << "YES no\n";
		else cout << "yes NO\n";

		cout << "\nFor switch press <-- -->\n";
		cout << "For continue press \"ENTER\"\n\n";

		InputSettings(Switch, key);

		system("cls");
	} while (key != 13);
	
	walls = Switch;
	
	// ��������� ���������������
	Switch = TRUE;

	do
	{
		cout << "Settings:\n";
		cout << "Self-crossing? ";

		if (Switch) cout << "YES no\n";
		else cout << "yes NO\n";

		cout << "\nFor switch press <-- -->\n";
		cout << "For continue press \"ENTER\"\n\n";

		InputSettings(Switch, key);

		system("cls");
	} while (key != 13);

	SCrossing = Switch;
}


void InputSettings(bool &Switch, short &key)
{
	key = _getch();

	switch (key)
	{
		// �����
		case 75:
		case 77:
			if (Switch) Switch  = FALSE;
			else Switch = TRUE;
			break;
	}
}


void Setup(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned long &score, const unsigned short &len,
		   const unsigned short &applec,
		   const char &SumbolField, const char &SumbolWall,
		   const char &SumbolSnakeHead, const char &SumbolSnakeTrunk)
{
	// ��������� ���� � ����
	for (int y = 0; y < heightY + 2; y++)
		for (int x = 0; x < widthX + 2; x++)
			if ((y == 0) || (y == heightY + 1) ||
				(x == 0) || (x == widthX + 1)) field[y][x] = SumbolWall;
			else field[y][x] = SumbolField;

			// ��������� ��������� ��������� ���� 
			snakeX[0] = widthX / 2;
			snakeY[0] = heightY / 2;
			snakeX[1] = widthX / 2;
			snakeY[1] = heightY / 2 - 1;

			// ������� ���� ��������� � ������ field
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
			field[snakeY[1]][snakeX[1]] = SumbolSnakeTrunk;

			// ��������� �����
			Draw(score, len, applec);
			Sleep(5000);
			system("cls");
}

/*
void Clock(short int tm)
{
	short int start = clock();
	short int end = start + tm;

	while (start <= end)
		start = clock();
}
*/


void Draw(const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec)
{
	system("cls");

	// ����� ����
	for (int y = 0; y < heightY + 2; y++)
	{
		for (int x = 0; x < widthX + 2; x++)
			cout << field[y][x];

		cout << '\n';
	}

	// ����� ��������� ����� � �����
	cout << '\n';
	cout << "Your score: " << score << '\n';
	cout << "Your lenght: " << len << '\n';
	cout << "You ate " << applec;
	if (applec != 1) cout << " apples.";
	else cout << " apple.";
	cout << '\n';

	// Debag
	/*
	cout << "\n\nKey: " << key << '\n';
	cout << "snakeX[0]: " << snakeX[0] << '\n';
	cout << "snakeY[0]: " << snakeY[0] << '\n';
	cout << "_x: " << _x << '\n';
	cout << "_y: " <<  _y << '\n';
	cout << "appleX: " << appleX << '\n';
	cout << "appleY: " << appleY << '\n';
	cout << "applec: " << applec << '\n';
	cout << "OldApplec: " << OldApplec << '\n';
	cout << "iteration: " << iteration << '\n';
	*/

	// ����� ��������� ���� (����������)
	cout << "\n    /\\\n";
	cout << "<-- || -->\n";
	cout << "    \\/\n";
	cout << "Press \"P\" to pause.\n";

	// ������ ��������� � ���. ����������
	cout << "\n\n";
	cout << "alpha 0.0.7 (VERY RAW Release with bugs)";
	cout << '\n';
	cout << "Copyright (c) 2016 Esoft. All rights reserved."; // excellent
	cout << '\n';
}


void Input(short &_x, short &_y, bool &GameOver)
{
	if (_kbhit())
	{
		unsigned short key = 80; // ����������� ���� (����-���) � �����
				  //int OldKey; // ���������� ����-��� ������� �������

		key = _getch();

		switch (key)
		{
			// ���� ����������� �������� ����
			case 75:
				_x = -1;
				_y = 0;
				break;
			case 77:
				_x = 1;
				_y = 0;
				break;
			case 72:
				_x = 0;
				_y = -1;
				break;
			case 80:
				_x = 0;
				_y = 1;
				break;
			// ����� (�� ������� � ����������)
			case 112:
			case 135:
			case 167:
				cout << "Pause\n" << "Press any key to continue\n";
				system("pause");
				break;
			case 27:
				GameOver = TRUE;
				break;
		}
	}
}


void GenerationObjects(const unsigned short &applec, unsigned short &OldApplec,
					   unsigned short &appleX, unsigned short &appleY,
					   const char &SumbolField, const char &SumbolApple)
{
	// ��������� �����
	if (OldApplec != applec)
	{
		for(;;)
		{
			srand(time(NULL));
		    appleX = rand() % widthX + 1;
			appleY = rand() % heightY + 1;
			
			// �������� �� ������ ������
			if (field[appleY][appleX] == SumbolField)
			{
				field[appleY][appleX] = SumbolApple;
				break;
			}
		}
		
        OldApplec = applec;
	}
}


void Move(unsigned short *snakeX, unsigned short *snakeY,
		  const short &_x, const short &_y,
		  const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec, const char &SumbolField,
		  const char &SumbolSnakeHead, const char &SumbolSnakeTrunk)
{
	unsigned short x, y;

     // ���������� ������ ����
	x = snakeX[len - 1];
	y = snakeY[len - 1];
	
	// ����������� ������ ���� (����� ������ � ������)
    // � ������ �� ����������� ���������� ��������� ������ ����   
	for (int i = len - 1; i > 0; i--)
	{
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}
	
	// ����������� ������ ����
	snakeX[0] = snakeX[0] + _x;
	snakeY[0] = snakeY[0] + _y;
	
	// ���������� ����������� ���� (� ������� field)
	field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
	field[snakeY[1]][snakeX[1]] = SumbolSnakeTrunk;
	field[y][x] = SumbolField;
}


void Logic(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned short &appleX, const unsigned short &appleY,
		   const bool &walls, const bool &SCrossing, bool &GameOver,
		   unsigned long &score, unsigned short &len, unsigned short &applec,
		   const char &SumbolWall, const char &SumbolSnakeHead)
{
	// �������� ��� �������� ������
	if ((snakeX[0] == appleX) && (snakeY[0] == appleY))
	{
		len++;
		applec++;
		score = score + 10;
		// �������� � ������� move() or � ����?
		return;
	}

	// �������� ��� ���������������
	if (len > 8)
		for (int i = 8; i < len; i++)
			if ((snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i]))
				if (SCrossing)
				{
					GameOver = TRUE;
					return;
				}
				else
				{
					len = i;
					return;
				}

	// �������� ��� ������������ �� ������
	if (walls)
	{
		if ((snakeX[0] == 0) || (snakeX[0] == widthX + 1) ||
			(snakeY[0] == 0) || (snakeY[0] == heightY + 1))
			GameOver = TRUE;
	}
	else
	{
		if (snakeX[0] == 0)
		{
			field[snakeY[0]][snakeX[0]] = SumbolWall;
			snakeX[0] = widthX;
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
		}
		else if (snakeX[0] == widthX + 1)
		{
			field[snakeY[0]][snakeX[0]] = SumbolWall;
			snakeX[0] = 1;
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
		}
		else if (snakeY[0] == 0)
		{
			field[snakeY[0]][snakeX[0]] = SumbolWall;
			snakeY[0] = heightY;
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
		}
		else if (snakeY[0] == heightY + 1)
		{
			field[snakeY[0]][snakeX[0]] = SumbolWall;
			snakeY[0] = 1;
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
		}
	}
}


void WinFunc() // �������� ���� ������ ��������� (0-4)
{

}


void ShuttingDown()
{
	system("pause");
}
