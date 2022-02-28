#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>

using namespace std;

// �� ����� �����
bool GameOver;
const int widthX = 40;
const int heightY = 20;
int key; // ����������� ���� (����-���)
//int OldKey; // ���������� ����-��� ������� �������

// ������������ ����������
int x, y; // ���������� (����� ������������ ��� ���������� ������)
int _x, _y; // ������������ ���� �� ����������� ���������� ����� �� ��������� �������� �����
int appleX, appleY; // ���������� ������

// ����������-��������
int len; // ����� ����
int score; // ���������
int applec; // ���� �����
int OldApplec; // OldApplec = applec - 1 (��� ��������� ������ ������)
int iteration; // ���������� �������� �� ���� ����
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
bool Switch; // ���������� �������������

// �������, ���������� �� ����
int snakeX[200], snakeY[200]; // ���������� ����
char field[heightY + 2][widthX + 3]; // field[y(������)][x(�������)]


// �������:
// ������ ��������� �������� ���� � �. �.
void Settings(); // ���� ��������� �������������
void InputSettings(); // ���� ��������
void Setup(); // ��������� ������� ����
void ShuttingDown(); // ���������� ���� (�������� ��� ����� ����?)

// ������� ����
void Draw(); // ������ ���� �������� �������� � ������� �������������� ����������
void Input(); // ���������� (����) ����� ���������
void Move(); // ����������� �������� ������� �� ���������� ������� �������
void GenerationObjects(); // ��������� ��������� (������, $, ��������� ������ � ����)
void Logic(); // ���������� ������� �������� �� ������������ ����� �������� � �����, �������� ����-����
void WinFunc(); // ���������� � ���������� ����������

int main()
{
	do
	{

		Settings();
		Setup();

		do
		{
			//iteration++; // Debug ����������

			Draw();
			Input();
			GenerationObjects();
			Move();
			Logic();

			Sleep(70); // ����� � ���������� ��������� �������� ��� �� ��������� ������� (3 ������)
		} while ((!GameOver) && (key != 27));

		if (GameOver) // ����� ������� ��������� �������
		{
			do
			{
				system("cls");
				cout << "Do you want to play again? ";

				if (Switch) cout << "YES no\n";
				else cout << "yes NO\n";

				cout << "\nFor switch press <-- -->\n";
				cout << "For continue press \"ENTER\"\n\n";

				InputSettings();
			} while (key != 13);

			GameOver = !Switch;
			system("cls");
		}
	} while ((!GameOver) && (key != 27));

	ShuttingDown();

	return 0;
}


void Settings()
{
	// ������� ������������� ������� ��� ������������, ��� ����� ��������
	// ������ cout << "��������� �� ������ ��� (������?) "
	// � InputSettings(walls(��������� ��� (������?))).


	// ��������� ����
	Switch = TRUE;

	do
	{
		cout << "Settings:\n";
		cout << "Walls? ";

		if (Switch) cout << "YES no\n";
		else cout << "yes NO\n";

		cout << "\nFor switch press <-- -->\n";
		cout << "For continue press \"ENTER\"\n\n";

		InputSettings();

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

		InputSettings();

		system("cls");
	} while (key != 13);

	SCrossing = Switch;
}


void InputSettings()
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


void Setup()
{
	// ��������� �������� ��������� ����������
	GameOver = FALSE;
	len = 2;
	score = applec = iteration = 0;
	key = 80;
	_x = 0;
	_y = 1;
	OldApplec = -1;

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
			Draw();
			Sleep(5000);
			system("cls");
}


void Draw()
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
	cout << "alpha 0.0.6.1 (Debug version)";
	cout << '\n';
	cout << "Copyright (c) 2016 Esoft. All rights reserved."; // excellent
	cout << '\n';
}


void Input()
{
	if (_kbhit())
	{
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


void GenerationObjects()
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


void Move()
{
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


void Logic()
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