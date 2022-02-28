#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>

using namespace std;

// Не имеют групп
bool GameOver;
const int widthX = 40;
const int heightY = 20;
int key; // Направление змеи (скан-код)
//int OldKey; // Предыдущий скан-код нажатой клавиши

// Координатные переменные
int x, y; // Координаты (также используются как переменные циклов)
int _x, _y; // Передвижение змеи на определённое количество шагов на следующей итерации цикла
int appleX, appleY; // Координаты яблока

// Переменные-счётчики
int len; // Длина змеи
int score; // Результат
int applec; // Счёт яблок
int OldApplec; // OldApplec = applec - 1 (для генерации нового яблока)
int iteration; // Количество итераций за одну игру
int GameTime; // Считает время игры
//int time; // Считает что-то временн`ое в игре ($)

// Char-переменные, использумые для обозначения символов вывода игрового интерфейса на экран
const char SumbolField = ' '; // Символ пустой клетки поля
const char SumbolWall = '#'; // Символ стены
const char SumbolSnakeHead = '0'; // Символ змеиной головы
const char SumbolSnakeTrunk = 'o'; // Символ змеиного туловища
const char SumbolApple = '@'; // Символ яблока
const char SumbolAppleInside = 'O'; // Символ проглоченного яблока

// Settings переменные
bool walls; // Наличие стен
bool SCrossing; // 1 - смерть, 0 - поедание хвоста
bool Switch; // переменная переключатель

// Массивы, отвечающие за игру
int snakeX[200], snakeY[200]; // Координаты змеи
char field[heightY + 2][widthX + 3]; // field[y(строка)][x(столбец)]


// ФУНКЦИИ:
// Фукции настройки главного меню и т. д.
void Settings(); // Меню настройки пользователем
void InputSettings(); // Ввод настроек
void Setup(); // Начальная настрой игры
void ShuttingDown(); // Завершение игры (основной или самой игры?)

// Функции игры
void Draw(); // Рисует поле заданных размеров и выводит дополнительную информацию
void Input(); // Управление (ввод) через стрелочки
void Move(); // Направление движение зависит от предыдущей нажатой клавиши
void GenerationObjects(); // Генерация предметов (яблоки, $, временная защита и мины)
void Logic(); // Логическая функция отвечает за происходящее после врезания в стену, съедания чего-либо
void WinFunc(); // Нахождение и объявление победителя

int main()
{
	do
	{

		Settings();
		Setup();

		do
		{
			//iteration++; // Debug информация

			Draw();
			Input();
			GenerationObjects();
			Move();
			Logic();

			Sleep(70); // Можно в настройках увеличить скорость или со следующем уровнем (3 уровня)
		} while ((!GameOver) && (key != 27));

		if (GameOver) // Потом сделать отдельную функцию
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
	// Создать универсальную функцию для переключения, где будет меняться
	// только cout << "указатель на строку или (ссылка?) "
	// и InputSettings(walls(указатель или (ссылка?))).


	// Настройка стен
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
	
	// Настройка самопересечения
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
		// Выбор
		case 75:
		case 77:
			if (Switch) Switch  = FALSE;
			else Switch = TRUE;
			break;
	}
}


void Setup()
{
	// Начальное значение некоторых переменных
	GameOver = FALSE;
	len = 2;
	score = applec = iteration = 0;
	key = 80;
	_x = 0;
	_y = 1;
	OldApplec = -1;

	// Генерация поля и стен
	for (int y = 0; y < heightY + 2; y++)
		for (int x = 0; x < widthX + 2; x++)
			if ((y == 0) || (y == heightY + 1) ||
				(x == 0) || (x == widthX + 1)) field[y][x] = SumbolWall;
			else field[y][x] = SumbolField;

			// Генерация начальных координат змеи 
			snakeX[0] = widthX / 2;
			snakeY[0] = heightY / 2;
			snakeX[1] = widthX / 2;
			snakeY[1] = heightY / 2 - 1;

			// Перенос этих координат в массив field
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
			field[snakeY[1]][snakeX[1]] = SumbolSnakeTrunk;

			// Начальный экран
			Draw();
			Sleep(5000);
			system("cls");
}


void Draw()
{
	system("cls");

	// Вывод поля
	for (int y = 0; y < heightY + 2; y++)
	{
		for (int x = 0; x < widthX + 2; x++)
			cout << field[y][x];

		cout << '\n';
	}

	// Вывод набранных очков и длины
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

	// Вывод элементов меню (управление)
	cout << "\n    /\\\n";
	cout << "<-- || -->\n";
	cout << "    \\/\n";
	cout << "Press \"P\" to pause.\n";

	// Версия программы и доп. информация
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
			// Ввод направления движения змеи
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
			// Пауза (на русском и английском)
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
	// Генерация яблок
	if (OldApplec != applec)
	{
		for(;;)
		{
			srand(time(NULL));
		    appleX = rand() % widthX + 1;
			appleY = rand() % heightY + 1;
			
			// Проверка на пустую ячейку
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
     // Коодринаты хвоста змеи
	x = snakeX[len - 1];
	y = snakeY[len - 1];
	
	// Перемешение частей змеи (кроме хвоста и головы)
    // с хвоста на последующие координаты следующих частей змеи   
	for (int i = len - 1; i > 0; i--)
	{
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}
	
	// Перемещение головы змеи
	snakeX[0] = snakeX[0] + _x;
	snakeY[0] = snakeY[0] + _y;
	
	// Визуальное перемещение змеи (в массиве field)
	field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
	field[snakeY[1]][snakeX[1]] = SumbolSnakeTrunk;
	field[y][x] = SumbolField;
}


void Logic()
{
	// Действия при съедании яблока
	if ((snakeX[0] == appleX) && (snakeY[0] == appleY))
	{
		len++;
		applec++;
		score = score + 10;
		// анимация в функции move() or в этой?
		return;
	}

	// Действие при самопересечении
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

	// Действия при столкновении со стеной
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


void WinFunc() // передача этой фукции аргумента (0-4)
{

}


void ShuttingDown()
{
	system("pause");
}