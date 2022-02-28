#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const unsigned short widthX = 40, heightY = 20;

// Ìàññèâû, îòâå÷àþùèé çà èãðó
char field[heightY + 2][widthX + 3]; // field[y(ñòðîêà)][x()]
unsigned short snakeX[200], snakeY[200]; // Êîîðäèíàòû çìåè

										 // ÔÓÍÊÖÈÈ:
										 // Ôóêöèè íàñòðîéêè ãëàâíîãî ìåíþ è ò. ä.
										 // void Clock(int tm);
void Settings(bool &walls, bool &SCrossing, short &key); // Ìåíþ íàñòðîéêè ïîëüçîâàòåëåì
void InputSettings(bool &Switch, short &key); // Ââîä íàñòðîåê
void Setup(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned long &score, const unsigned short &len,
		   const unsigned short &applec,
		   const char &SumbolField, const char &SumbolWall,
		   const char &SumbolSnakeHead, const char &SumbolSnakeTrunk); // Íà÷àëüíàÿ íàñòðîé èãðû
void ShuttingDown(); // Çàâåðøåíèå èãðû

					 // Ôóíêöèè èãðû
void Draw(const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec); // Ðèñóåò ïîëå çàäàííûõ ðàçìåðîâ è âûâîäèò äîïîëíèòåëüíóþ èíôîðìàöèþ

void Input(short &_x, short &_y, bool &GameOver); // Óïðàâëåíèå (ââîä) ÷åðåç ñòðåëî÷êè

void GenerationObjects(const unsigned short &applec, unsigned short &OldApplec,
					   unsigned short &appleX, unsigned short &appleY,
					   const char &SumbolField, const char &SumbolApple); // Ãåíåðàöèÿ ïðåäìåòîâ (ÿáëîêè, $, âðåìåííàÿ çàùèòà è ìèíû)

void Move(unsigned short *snakeX, unsigned short *snakeY, const short &_x, const short &_y,
		  const unsigned long &score, const unsigned short &len,
		  const unsigned short &applec, const char &SumbolField,
		  const char &SumbolSnakeHead, const char &SumbolSnakeTrunk); // Íàïðàâëåíèå äâèæåíèå çàâèñèò îò ïðåäûäóùåé íàæàòîé êëàâèøè

void Logic(unsigned short *snakeX, unsigned short *snakeY,
		   const unsigned short &appleX, const unsigned short &appleY,
		   const bool &walls, const bool &SCrossing, bool &GameOver,
		   unsigned long &score, unsigned short &len, unsigned short &applec,
		   const char &SumbolWall, const char &SumbolSnakeHead); // Ëîãè÷åñêàÿ ôóíêöèÿ îòâå÷àåò çà ïðîèñõîäÿùåå ïîñëå âðåçàíèÿ â ñòåíó, ñúåäàíèÿ ÷åãî-ëèáî

void WinFunc(); // Íàõîæäåíèå è îáúÿâëåíèå ïîáåäèòåëÿ





				// ñòðóêòóðà äëÿ êîììàíä ââîäà
struct command
{
	bool OldChoose; // ïðåäûäóùèé âûáîð (TRUE - áûë ñäåëàí, FALSE - íå áûë ñäåëàí)
	short choose; // âûáîð
	bool esc, back, enter;
};


void MainMenu(); // ãëàâíîå ìåíþ
void Play(command &com); // èãðàòü
void Settings(command &com); // íàñòðîéêè
void Help(command &com); // ïîìîùü


						 // ÔÓÍÊÖÈß ÐÈÑÎÂÀÍÈß ÎÊÍÀ
void DrawWindow(char **str, const unsigned short &size, const unsigned short &BottomSize = 0);

// ÔÓÍÊÖÈÈ ÂÛÁÎÐÀ Â ÐÀÇËÈ×ÍÛÕ ÌÅÍÞ
void ChooseY(command &com, const unsigned short &num = 2); // ôóíêöèÿ âåðòèêàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ
void ChooseX();  // ôóíêöèÿ ãîðèçîíòàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ

				 // ÔÓÍÊÖÈÈ ÐÈÑÎÂÀÍÈß ÑÒÅÍ  Â ÌÅÍÞ
void WallX(const unsigned short &num = 80); // ôóíêöèÿ ðèñîâàíèÿ ãîðèçîíòàëüíîé ñòåíû â ìåíþ
void WallY(const unsigned short &num = 1); // ôóíêöèÿ ðèñîâàíèÿ âåðòèêàëüíîé ñòåíû â ìåíþ

void Space(const unsigned short &num = 1); // ôóíêöèÿ ðèñîâàíèÿ ïðîáåëîâ â ìåíþ

int main()
{
	MainMenu();

	return 0;
}


/////////////////////////////////////////////////
//               ÃËÀÂÍÎÅ ÌÅÍÞ                  //
/////////////////////////////////////////////////

void MainMenu()
{
	const unsigned short size = 5;
	command com; // command (êîìàíäà)
	char *str[size];
	char s[size][9];

	com.choose = 1;

	for (int i = 0; i < size; i++) str[i] = s[i];

	// MainMenu
	do
	{
		com.OldChoose = com.esc = com.back = com.enter = FALSE;

		strcpy(s[0], "SNAKE");
		(com.choose == 1) ? strcpy(s[1], "PLAY") : strcpy(s[1], "Play");
		(com.choose == 2) ? strcpy(s[2], "SETTINGS") : strcpy(s[2], "Settings");
		(com.choose == 3) ? strcpy(s[3], "HELP") : strcpy(s[3], "Help");
		(com.choose == 4) ? strcpy(s[4], "EXIT") : strcpy(s[4], "Exit");

		// Âûâîä ãëàâíîãî ìåíþ
		DrawWindow(str, size, 2);
		WallY(2);
		cout << " Creator:                                                                    |\n";
		cout << "| Vladislav Melnichuk            (c) 2016 Esoft                       v 0.8.0 |\n";
		WallX();

		do
		{
			ChooseY(com, 4);

			// Åñëè áûë ñäåëàí âûáîð, âõîä â äðóãîå ìåíþ
			if (com.enter)
				switch (com.choose)
				{
					case 1:
						Play(com);
						break;
					case 2:
						Settings(com);
						break;
					case 3:
						Help(com);
				}
		} while (!((com.back && (com.choose == 4)) || com.enter || com.OldChoose));
	} while (!((com.choose == 4) && (com.back || com.enter)));
}


/////////////////////////////////////////////////
//            ÔÓÍÊÖÈÈ ÃËÀÂÍÎÃÎ ÌÅÍÞ            //
/////////////////////////////////////////////////

// èãðàòü
void Play(command &com)
{
	bool GameOver = FALSE; // Îêîí÷àíèå èãðû
	short key = 80; // Íàïðàâëåíèå çìåè (ñêàí-êîä), ïàóçà (îñòàëüíûå íàñòðîéêè â îòäåëüíóþ ôóíêöèþ)

	do
	{
		// Êîîðäèíàòíûå ïåðåìåííûå
		short _x = 0, _y = 1; // Ïåðåäâèæåíèå çìåè íà îïðåäåë¸ííîå êîëè÷åñòâî øàãîâ íà ñëåäóþùåé èòåðàöèè öèêëà
		unsigned short appleX, appleY; // Êîîðäèíàòû ÿáëîêà

									   // Ïåðåìåííûå-ñ÷¸ò÷èêè
		unsigned short len = 2; // Äëèíà çìåè
		unsigned long score = 0; // Ðåçóëüòàò
		unsigned short applec = 0; // Ñ÷¸ò ÿáëîê
		unsigned short OldApplec = 1; // OldApplec = applec - 1 (äëÿ ãåíåðàöèè íîâîãî ÿáëîêà)
									  // unsigned long iteration = 0; // Êîëè÷åñòâî èòåðàöèé çà îäíó èãðó
		int GameTime; // Ñ÷èòàåò âðåìÿ èãðû
					  //int time; // Ñ÷èòàåò ÷òî-òî âðåìåíí`îå â èãðå ($)


					  // Char-ïåðåìåííûå, èñïîëüçóìûå äëÿ îáîçíà÷åíèÿ ñèìâîëîâ âûâîäà èãðîâîãî èíòåðôåéñà íà ýêðàí
		const char SumbolField = ' '; // Ñèìâîë ïóñòîé êëåòêè ïîëÿ
		const char SumbolWall = '#'; // Ñèìâîë ñòåíû
		const char SumbolSnakeHead = '0'; // Ñèìâîë çìåèíîé ãîëîâû
		const char SumbolSnakeTrunk = 'o'; // Ñèìâîë çìåèíîãî òóëîâèùà
		const char SumbolApple = '@'; // Ñèìâîë ÿáëîêà
		const char SumbolAppleInside = 'O'; // Ñèìâîë ïðîãëî÷åííîãî ÿáëîêà

											// Settings ïåðåìåííûå
		bool walls; // Íàëè÷èå ñòåí
		bool SCrossing; // 1 - ñìåðòü, 0 - ïîåäàíèå õâîñòà


		Settings(walls, SCrossing, key);
		Setup(snakeX, snakeY, score, len, applec, SumbolField,
			  SumbolWall, SumbolSnakeHead, SumbolSnakeTrunk);

		do
		{
			//iteration++; // Debug èíôîðìàöèÿ

			Draw(score, len, applec); // íóæåí äèíàìè÷åñêèé äâóìåðíûé ìàññèâ
			Input(_x, _y, GameOver);
			GenerationObjects(applec, OldApplec, appleX, appleY,
							  SumbolField, SumbolApple);
			Move(snakeX, snakeY, _x, _y, score, len, applec, SumbolField,
				 SumbolSnakeHead, SumbolSnakeTrunk);
			Logic(snakeX, snakeY, appleX, appleY, walls, SCrossing,
				  GameOver, score, len, applec, SumbolWall, SumbolSnakeHead);

			Sleep(100); // Ìîæíî â íàñòðîéêàõ óâåëè÷èòü ñêîðîñòü èëè ñî ñëåäóþùèì óðîâíåì (3 óðîâíÿ)
		} while ((!GameOver) && (key != 27));

		if (GameOver) // Ïîòîì ñäåëàòü îòäåëüíóþ ôóíêöèþ
		{
			bool Switch = TRUE; // ïåðåäåëàòü
			Sleep(5000);

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
}

// íàñòðîéêè
void Settings(command &com)
{

}

// ïîìîùü
void Help(command &com)
{

}


/////////////////////////////////////////////////
//       ÔÓÍÊÖÈÈ ÂÛÁÎÐÀ Â ÐÀÇËÈ×ÍÛÕ ÌÅÍÞ       //
/////////////////////////////////////////////////

// ôóíêöèÿ âåðòèêàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ
void ChooseY(command &com, const unsigned short &num)
{
	short key = getch();

	// ïðîâåðêà íà âõîä â äðóãîå ìåíþ
	if (key == 13) com.enter = TRUE;
	else if (key == 27) com.esc = TRUE;

	else if (key == 224)
	{
		key = getch();

		switch (key)
		{
			// Âûáîð
			case 72:
				if (com.choose == 1) com.choose = num;
				else com.choose--;
				com.OldChoose = TRUE;
				break;
			case 80:
				if (com.choose == num) com.choose = 1;
				else com.choose++;
				com.OldChoose = TRUE;
				break;
			case 77:
				com.enter = TRUE;
				break;
			case 75:
				com.back = TRUE;
		}
	}
}

// ôóíêöèÿ ãîðèçîíòàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ
void ChooseX()
{

}



/////////////////////////////////////////////////
//                                             //
/////////////////////////////////////////////////

// ÔÓÍÊÖÈß ÐÈÑÎÂÀÍÈß ÎÊÍÀ
void DrawWindow(char **str, const unsigned short &size, const unsigned short &BottomSize)
{
	// for (int i = 0; i < 8; i++) cout << "1234567890";
	system("cls");

	short space = 38 - strlen(str[0]) / 2;

	WallX();
	cout << '\n';
	WallY();
	Space(space);
	cout << str[0];
	Space(77 - (space + strlen(str[0])));
	WallY(2);
	Space(77);
	WallY(2);

	for (int i = 1; i < size; i++)
	{
		Space(space);
		cout << str[i];
		Space(77 - (space + strlen(str[i])));
		WallY(2);
	}

	Space(77);

	for (int i = 0; i < (21 - size - BottomSize); i++)
	{
		WallY(2);
		Space(77);
	}

	if (!BottomSize)
	{
		WallY();
		cout << '\n';
		WallX();
	}
}


/////////////////////////////////////////////////
//   ÔÓÍÊÖÈÈ ÐÈÑÎÂÀÍÈß ÑÒÅÍ È ÏÐÎÁÅËÎÂ Â ÌÅÍÞ  //
/////////////////////////////////////////////////

// ôóíêöèÿ ðèñîâàíèÿ ãîðèçîíòàëüíîé ñòåíû â ìåíþ
void WallX(const unsigned short &num)
{
	cout << '+';
	for (int i = 0; i < (num - 3); i++) cout << '-';
	cout << '+';
}

// ôóíêöèÿ ðèñîâàíèÿ âåðòèêàëüíîé ñòåíû â ìåíþ
void WallY(const unsigned short &num)
{
	for (int i = 0; i < (num - 1); i++) cout << "|\n";
	cout << '|';
}

// ôóíêöèÿ ðèñîâàíèÿ ïðîáåëîâ â ìåíþ
void Space(const unsigned short &num)
{
	for (int i = 0; i < num; i++) cout << ' ';
}


/////////////////////////////////////////////////
//                                             //
/////////////////////////////////////////////////










void Settings(bool &walls, bool &SCrossing, short &key)
{
	bool Switch = TRUE; // ïåðåìåííàÿ ïåðåêëþ÷àòåëü

						// Ñîçäàòü óíèâåðñàëüíóþ ôóíêöèþ äëÿ ïåðåêëþ÷åíèÿ, ãäå áóäåò ìåíÿòüñÿ
						// òîëüêî cout << "óêàçàòåëü íà ñòðîêó èëè (ññûëêà?) "
						// è InputSettings(walls(óêàçàòåëü èëè (ññûëêà?))).

	do
	{
		system("cls");
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

	// Íàñòðîéêà ñàìîïåðåñå÷åíèÿ
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
		// Âûáîð
		case 75:
		case 77:
			if (Switch) Switch = FALSE;
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
	// Ãåíåðàöèÿ ïîëÿ è ñòåí
	for (int y = 0; y < heightY + 2; y++)
		for (int x = 0; x < widthX + 2; x++)
			if ((y == 0) || (y == heightY + 1) ||
				(x == 0) || (x == widthX + 1)) field[y][x] = SumbolWall;
			else field[y][x] = SumbolField;

			// Ãåíåðàöèÿ íà÷àëüíûõ êîîðäèíàò çìåè 
			snakeX[0] = widthX / 2;
			snakeY[0] = heightY / 2;
			snakeX[1] = widthX / 2;
			snakeY[1] = heightY / 2 - 1;

			// Ïåðåíîñ ýòèõ êîîðäèíàò â ìàññèâ field
			field[snakeY[0]][snakeX[0]] = SumbolSnakeHead;
			field[snakeY[1]][snakeX[1]] = SumbolSnakeTrunk;

			// Íà÷àëüíûé ýêðàí
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

	// Âûâîä ïîëÿ
	for (int y = 0; y < heightY + 2; y++)
	{
		for (int x = 0; x < widthX + 2; x++)
			cout << field[y][x];

		cout << '\n';
	}

	// Âûâîä íàáðàííûõ î÷êîâ è äëèíû
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

	// Âûâîä ýëåìåíòîâ ìåíþ (óïðàâëåíèå)
	cout << "\n    /\\\n";
	cout << "<-- || -->\n";
	cout << "    \\/\n";
	cout << "Press \"P\" to pause.\n";

	// Âåðñèÿ ïðîãðàììû, ëîãîòèï è ñîçäàòåëü
	cout << "\n\n";
	cout << " Creator:                                                                    \n";
	cout << " Vladislav Melnichuk            (c) 2016 Esoft                       v 0.8.0 \n";
}


void Input(short &_x, short &_y, bool &GameOver)
{
	if (_kbhit())
	{
		unsigned short key = 80; // Íàïðàâëåíèå çìåè (ñêàí-êîä) è ïàóçû

		key = getch();

		switch (key)
		{
			// Ïàóçà (íà ðóññêîì è àíãëèéñêîì)
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

		if (key == 224)
		{
			key = getch();

			switch (key)
			{
				// Ââîä íàïðàâëåíèÿ äâèæåíèÿ çìåè
				case 75:
					if (_x != 1)
					{
						_x = -1;
						_y = 0;
					}
					break;
				case 77:
					if (_x != -1)
					{
						_x = 1;
						_y = 0;
					}
					break;
				case 72:
					if (_y != 1)
					{
						_x = 0;
						_y = -1;
					}
					break;
				case 80:
					if (_y != -1)
					{
						_x = 0;
						_y = 1;
					}
					break;
			}
		}
	}
}


void GenerationObjects(const unsigned short &applec, unsigned short &OldApplec,
					   unsigned short &appleX, unsigned short &appleY,
					   const char &SumbolField, const char &SumbolApple)
{
	// Ãåíåðàöèÿ ÿáëîê
	if (OldApplec != applec)
	{
		for (;;)
		{
			srand(time(NULL));
			appleX = rand() % widthX + 1;
			appleY = rand() % heightY + 1;

			// Ïðîâåðêà íà ïóñòóþ ÿ÷åéêó
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

	// Êîîäðèíàòû õâîñòà çìåè
	x = snakeX[len - 1];
	y = snakeY[len - 1];

	// Ïåðåìåøåíèå ÷àñòåé çìåè (êðîìå õâîñòà è ãîëîâû)
	// ñ õâîñòà íà ïîñëåäóþùèå êîîðäèíàòû ñëåäóþùèõ ÷àñòåé çìåè   
	for (int i = len - 1; i > 0; i--)
	{
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}

	// Ïåðåìåùåíèå ãîëîâû çìåè
	snakeX[0] = snakeX[0] + _x;
	snakeY[0] = snakeY[0] + _y;

	// Âèçóàëüíîå ïåðåìåùåíèå çìåè (â ìàññèâå field)
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
	// Äåéñòâèÿ ïðè ñúåäàíèè ÿáëîêà
	if ((snakeX[0] == appleX) && (snakeY[0] == appleY))
	{
		len++;
		applec++;
		score = score + 10;
		// àíèìàöèÿ â ôóíêöèè move() or â ýòîé?
		return;
	}

	// Äåéñòâèå ïðè ñàìîïåðåñå÷åíèè
	if (len > 4)
		for (int i = 4; i < len; i++)
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

	// Äåéñòâèÿ ïðè ñòîëêíîâåíèè ñî ñòåíîé
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


void WinFunc() // ïåðåäà÷à ýòîé ôóêöèè àðãóìåíòà (0-4)
{

}


void ShuttingDown()
{
	system("pause");
}