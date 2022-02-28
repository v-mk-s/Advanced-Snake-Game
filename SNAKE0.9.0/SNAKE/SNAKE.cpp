#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

using namespace std;


// ñòðóêòóðà äëÿ êîììàíä ââîäà
struct command
{
	unsigned short choose; // âûáîð
    bool OldChoose; // ïðåäûäóùèé âûáîð (TRUE - áûë ñäåëàí, FALSE - íå áûë ñäåëàí)
	bool esc; // âûõîä â ãëàâíîå ìåíþ
	bool back; // âåðíóòüñÿ íàçàä
	bool enter; // âûáîð
	bool f; // âûõîä èç ôóíêöèè
};

struct options
{
	bool music; // ìóçûêà â èãðå
	bool game_sounds; // çâóêè â èãðå (ïîçæå ðàçäåëèòü íà çâóêè îò îáúåêòîâ, çìåè è ò. ä.)
};

struct statistics
{
	// ïîçæå ñ÷èòûâàíèå èç ôàéëà
	unsigned short MaxLen; // ìàêñèìàëüíàÿ äëèíà çìåè çà îäíó èãðó
	unsigned short MaxScore; // ìàêñèìàëüíîå êîë-âî íàáðàííûõ î÷êîâ çà îäíó èãðó
	unsigned short MaxApple; // ìàêñèìàëüíîå êîë-âî ñúåäåííûõ ÿáëîê çà îäíó èãðó
	unsigned long AllApple; // îáùåå êîë-âî ñúåäåííûõ ÿáëîê
	unsigned short MaxGoldApple; // ìàêñèìàëüíîå êîë-âî çîëîòûõ ÿáëîê, êîòîðûå áûëè ñúåäåíû çà îäíó èãðó
	unsigned long AllGoldApple; // îáùåå êîë-âî çîëîòûõ ÿáëîê, êîòîðûå áûëè ñúåäåíû
	unsigned long MaxGameTime; // âðåìÿ èãðû â ñåêóíäàõ çà îäíó èãðó
	long double AllGameTime; // îáùåå âðåìÿ èãðû â ÷àñàõ
	unsigned short GameQuit; // êîë-âî âûõîäîâ èç èãðû
	unsigned short AllDeath; // îáùåå êîë-âî ñìåðòåé
	// èç íèõ îò ñòåíû
	unsigned short WallDeath; // îáùåå êîë-âî ñìåðòåé îò ñòåíû
	// îò ñàìîïåðåñå÷åíèÿ
	unsigned short SCrossingDeath; // îáùåå êîë-âî ñìåðòåé îò ñàìîïåðåñå÷åíèÿ
	
};

// ÔÓÍÊÖÈÈ ÃËÀÂÍÎÃÎ ÌÅÍÞ
void MainMenu(); // ãëàâíîå ìåíþ
void Play(command &com, options &opt); // èãðàòü
void Options(command &com, options &opt); // îïöèè
void Achievements(command &com); // äîñòèæåíèÿ
void Statistics(command &com, statistics stat); // ñòàòèñòèêà
void Leaderboard(command &com); // ëó÷øèå ðåçóëüòàòû èãðîêîâ
void Creator(command &com); // ñîçäàòåëü
void Help(command &com); // ïîìîùü


// ÔÓÍÊÖÈß ÐÈÑÎÂÀÍÈß ÎÊÍÀ
void DrawWindow(char **str, const unsigned short &size, const unsigned short &BottomSize = 0);
/*
   char **str - óêàçàòåëü íà ìàññèâ ñòðîê, êîòîðûå íóæíî âûâåñòè êàê ìåíþ
   const unsigned short &size - êîëè÷åñòâî ýòèõ ñòðîê
   const unsigned short &BottomSize - êîëè÷åñòâî ñòðîê, êîòîðûå íàäî ïðîïóñòèòü
   ñíèçó ïðè ðèñîâàíèè ìåíþ
*/

// ÔÓÍÊÖÈÈ ÂÛÁÎÐÀ Â ÐÀÇËÈ×ÍÛÕ ÌÅÍÞ
void ChooseY(command &com, const unsigned short &num, const bool &h = 0); // ôóíêöèÿ âåðòèêàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ
/*
   const unsigned short &num - ìàêñèìàëüíîå ÷èñëî  äàííîãî ñïèñêà (const unsigned short &size - êîëè÷åñòâî ýòèõ ñòðîê)
   const bool &h - TRUE (âûçîâ áûë ñäåëàí èç ôóíêöèè Help) else FALSE
*/

void ChooseX(command &com, const unsigned short &num, const unsigned short &chooseX = 2);  // ôóíêöèÿ ãîðèçîíòàëüíîãî âûáîðà ðàçëè÷íûõ ìåíþ
// const unsigned short &num - ìàêñèìàëüíîå ÷èñëî  äàííîãî ñïèñêà (const unsigned short &size - êîëè÷åñòâî ýòèõ ñòðîê)

// ÔÓÍÊÖÈÈ ÐÈÑÎÂÀÍÈß ÑÒÅÍ È ÏÐÎÁÅËÎÂ Â ÌÅÍÞ
void WallX(const unsigned short &num = 80); // ôóíêöèÿ ðèñîâàíèÿ ãîðèçîíòàëüíîé ñòåíû â ìåíþ
// const unsigned short &num - äëèíà ñòåíû, íàðèñîâàííîé ïîñëå âûçîâà ýòîé ôóíêöèè

void WallY(const unsigned short &num = 1); // ôóíêöèÿ ðèñîâàíèÿ âåðòèêàëüíîé ñòåíû â ìåíþ
// const unsigned short &num - äëèíà ñòåíû, íàðèñîâàííîé ïîñëå âûçîâà ýòîé ôóíêöèè

void Space(const unsigned short &num = 1); // ôóíêöèÿ ðèñîâàíèÿ ïðîáåëîâ â ìåíþ
// const unsigned short &num - êîë-âî íåîáõîäèìûõ ïðîáåëîâ


/////////////////////////////////// ÔÓÍÊÖÈÈ ÈÃÐÛ /////////////////////////////////////////////
void Classic_80s(options &opt);
void Classic_90s(options &opt);
void Level_mode(options &opt);
void Esoft_mode(options &opt);
void Custom_mode(options &opt);

//////////////////////////////////////////////////////////////////////////////////////////////


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
	command com; // com (êîìàíäà)
	options opt; // opt (îïöèè)
	statistics stat; // stat (ñòàòèñòèêà) 

	opt.music = opt.game_sounds = TRUE; // ïîçæå ñ÷èòûâàíèå èç ôàéëà
	// ïîçæå ñ÷èòûâàíèå èç ôàéëà
	stat.MaxLen = stat.MaxScore = stat.MaxApple = stat.AllApple = stat.MaxGoldApple
		= stat.AllGoldApple = stat.MaxGameTime = stat.AllGameTime = stat.GameQuit
		= stat.AllDeath = stat.WallDeath = stat.SCrossingDeath = 0;

	const unsigned short size = 9;
	char *str[size]; 
	char s[size][13];
	
	com.choose = 1;
		
	for(int i = 0; i < size; i++) str[i] = s[i];
	
	// MainMenu
	strcpy(s[0], "SNAKE");

	do
	{
		com.OldChoose = com.esc = com.enter = com.f = FALSE;

		
		(com.choose == 1) ? strcpy(s[1], "PLAY") : strcpy(s[1], "Play");
		(com.choose == 2) ? strcpy(s[2], "OPTIONS") : strcpy(s[2], "Options");
		(com.choose == 3) ? strcpy(s[3], "ACHIEVEMENTS") : strcpy(s[3], "Achievements");
		(com.choose == 4) ? strcpy(s[4], "STATISTICS") : strcpy(s[4], "Statistics");
		(com.choose == 5) ? strcpy(s[5], "LEADERBOARD") : strcpy(s[5], "Leaderboard");
		(com.choose == 6) ? strcpy(s[6], "CREATOR") : strcpy(s[6], "Creator");
		(com.choose == 7) ? strcpy(s[7], "HELP") : strcpy(s[7], "Help");
		(com.choose == 8) ? strcpy(s[8], "EXIT") : strcpy(s[8], "Exit");
		
		// Âûâîä ãëàâíîãî ìåíþ
		DrawWindow(str, size, 2);
		WallY(2);
		cout <<  " Creator:                                                                    |\n";
		cout << "| Vladislav Melnichuk            (c) 2016 Esoft                       v 0.9.2 |\n";
		WallX();
		
		do
		{
			com.back = FALSE;

		    ChooseY(com, 8);
			
			// Åñëè áûë ñäåëàí âûáîð, âõîä â äðóãîå ìåíþ
		    if (com.enter)
			    switch (com.choose)
			    {
				    case 1:
					    Play(com, opt);
					    break;
	 				case 2:
					    Options(com, opt);
						break;
					case 3:
						Achievements(com);
						break;
					case 4:
						Statistics(com, stat);
						break;
					case 5:
						Leaderboard(com);
						break;
					case 6:
						Creator(com);
						break;
		            case 7:
					    Help(com);
			    }
		} while (!((com.back && (com.choose == 8)) || com.enter || com.OldChoose || com.f)); 
	} while (!((com.choose == 8) && (com.back || com.enter)));
}


/////////////////////////////////////////////////
//            ÔÓÍÊÖÈÈ ÃËÀÂÍÎÃÎ ÌÅÍÞ            //
/////////////////////////////////////////////////

// èãðàòü
void Play(command &com, options &opt)
{
	const unsigned short size = 11;
	char *str[size];
	char s[size][12];

	com.choose = 1;

	for (int i = 0; i < size; i++) str[i] = s[i];

	// Play
	strcpy(s[0], "   Play:");
	strcpy(s[1], "");
	strcpy(s[3], "");
	strcpy(s[5], "");
	strcpy(s[7], "");
	strcpy(s[9], "");

	do
	{
		com.OldChoose = com.esc = com.back = com.enter = com.f = FALSE;

		(com.choose == 1) ? strcpy(s[2], "CLASSIC 80S") : strcpy(s[2], "Classic 80s");
		(com.choose == 2) ? strcpy(s[4], "CLASSIC 90S") : strcpy(s[4], "Classic 90s");
		(com.choose == 3) ? strcpy(s[6], "LEVEL MODE") : strcpy(s[6], "Level mode");
		(com.choose == 4) ? strcpy(s[8], "ESOFT MODE") : strcpy(s[8], "Esoft mode");
		(com.choose == 5) ? strcpy(s[10], "CUSTOM MODE") : strcpy(s[10], "Custom mode");

		// Âûâîä ìåíþ Play
		DrawWindow(str, size);

		do
		{
			ChooseY(com, 5);

			// Åñëè áûë ñäåëàí âûáîð, âõîä â äðóãîå ìåíþ
			if (com.enter)
				switch (com.choose)
				{
					case 1:
						Classic_80s(opt);
						com.choose = 1;
						com.f = TRUE;
						break;
					case 2:
						Classic_90s(opt);
						com.choose = 2;
						com.f = TRUE;
						break;
					case 3:
						Level_mode(opt);
						com.choose = 3;
						com.f = TRUE;
						break;
					case 4:
						Esoft_mode(opt);
						com.choose = 4;
						com.f = TRUE;
						break;
					case 5:
						Custom_mode(opt);
						com.choose = 5;
						com.f = TRUE;
				}
		} while (!(com.back || com.OldChoose || com.esc || com.f));
	} while (!(com.back || com.esc));

	com.choose = 1;
	com.f = TRUE;
}

// íàñòðîéêè
void Options(command &com, options &opt)
{
	const unsigned short size = 5;
	char *str[size];
	char s[size][15];

	com.choose = 1;

	for (int i = 0; i < size; i++) str[i] = s[i];

	strcpy(s[0], "Settings:");

	do
	{
		com.OldChoose = com.esc = com.back = com.enter = FALSE;

		// Ñïèñîê íàñòðîåê
		if (opt.music == TRUE) strcpy(s[1], "Music: V");
		else strcpy(s[1], "Music:");
		if (com.choose == 1) strcpy(s[2], "YES no");
		else if (com.choose == 2) strcpy(s[2], "yes NO");
		else strcpy(s[2], "yes no");

		if (opt.game_sounds == TRUE) strcpy(s[3], "Game sounds: V");
		else strcpy(s[3], "Game sounds:");
		if (com.choose == 3) strcpy(s[4], "YES no");
		else if (com.choose == 4) strcpy(s[4], "yes NO");
		else strcpy(s[4], "yes no");

		// Âûâîä ñïèñêà íàñòðîåê
		DrawWindow(str, size);

		do
		{
			ChooseX(com, 4);

			// Åñëè áûë ñäåëàí âûáîð, âõîä â äðóãîå ìåíþ
			if (com.enter)
				switch (com.choose)
				{
					case 1:
						if (opt.music == FALSE) opt.music = TRUE;
						else com.enter = FALSE;
						break;
					case 2:
						if (opt.music == TRUE) opt.music = FALSE;
						else com.enter = FALSE;
						break;
					case 3:
						if (opt.game_sounds == FALSE) opt.game_sounds = TRUE;
						else com.enter = FALSE;
						break;
					case 4:
						if (opt.game_sounds == TRUE) opt.game_sounds = FALSE;
						else com.enter = FALSE;
				}
		} while (!(com.back || com.OldChoose || com.esc || com.enter));
	} while (!(com.back || com.esc));

	com.choose = 2;
	com.f = TRUE;
}

// äîñòèæåíèÿ
void Achievements(command &com)
{
	com.choose = 3;
	com.f = TRUE;
}

// ñòàòèñòèêà
void Statistics(command &com, statistics stat)
{
	const unsigned short size = 13;
	char *str[size];
	char s[size][70];
	char ia[26];

	for (int i = 0; i < size; i++) str[i] = s[i];

	// Statistics
	strcpy(s[0], "                    Statistics:                                      ");

	strcpy(s[1], "The maximum length: ");
	itoa(stat.MaxLen, ia, 10);
	strcat(s[1], ia);
	strcat(s[1], " centimeters");

	strcpy(s[2], "The maximum score: ");
	itoa(stat.MaxScore, ia, 10);
	strcat(s[2], ia);

	strcpy(s[3], "The maximum number of eaten apples: ");
	itoa(stat.MaxLen, ia, 10);
	strcat(s[3], ia);
	strcat(s[3], " (");
	// ïîçæå âûâîä double çíà÷åíèÿ
	itoa(((int) stat.MaxLen * 0.176), ia, 10); // 0.176 - ñðåäíèé âåñ îäíîãî ÿáëîêà (â áóäóþùåì ìîæíî áóäåò èçìåíÿòü â íàñòðîéêàõ :))
	strcat(s[3], ia);
	strcat(s[3], " kilograms)");

	strcpy(s[4], "The total number of eaten apples: ");
	ltoa(stat.AllApple, ia, 10);
	strcat(s[4], ia);
	strcat(s[4], " (");
	// ïîçæå âûâîä double çíà÷åíèÿ
	ltoa(((int) stat.AllApple * 0.176), ia, 10); // 0.176 - ñðåäíèé âåñ îäíîãî ÿáëîêà (â áóäóþùåì ìîæíî áóäåò èçìåíÿòü â íàñòðîéêàõ)
	strcat(s[4], ia);
	strcat(s[4], " kilograms)");

	strcpy(s[5], "The maximum number of eaten golden apples: ");
	itoa(stat.MaxGoldApple, ia, 10);
	strcat(s[5], ia);
	strcat(s[5], " (");
	// ïîçæå âûâîä double çíà÷åíèÿ
	itoa(((int) stat.MaxGoldApple * 4.246), ia, 10); // 4.246 - ñðåäíèé âåñ îäíîãî çîëîòîãî ÿáëîêà (â áóäóþùåì ìîæíî áóäåò èçìåíÿòü â íàñòðîéêàõ)
	strcat(s[5], ia);
	strcat(s[5], " kilograms)");

	strcpy(s[6], "The total number of eaten  golden apples: ");
	ltoa(stat.AllGoldApple, ia, 10);
	strcat(s[6], ia);
	strcat(s[6], " (");
	// ïîçæå âûâîä double çíà÷åíèÿ
	ltoa(((int) stat.AllGoldApple * 4.246), ia, 10); // 4.246 - ñðåäíèé âåñ îäíîãî çîëîòîãî ÿáëîêà (â áóäóþùåì ìîæíî áóäåò èçìåíÿòü â íàñòðîéêàõ)
	strcat(s[6], ia);
	strcat(s[6], " kilograms)");

	strcpy(s[7], "The maximum game time: ");
	if (stat.MaxGameTime == 1) strcat(s[7], "a second");
	else
	{
		ltoa(stat.MaxGameTime, ia, 10);
		strcat(s[7], ia);
		strcat(s[7], " seconds");
	}
	strcat(s[7], " (");
	// ïîçæå âûâîä double çíà÷åíèÿ
	if (stat.MaxGameTime == 60) strcat(s[7], "a minute)");
	else
	{
		ltoa(((int) stat.MaxGameTime / 60), ia, 10);
		strcat(s[7], ia);
		strcat(s[7], " minutes)");
	}

	strcpy(s[8], "All game time: ");
	if (stat.AllGameTime == 1) strcat(s[8], "an hour");
	else
	{
		ltoa(stat.MaxGameTime, ia, 10);
		strcat(s[8], ia);
		strcat(s[8], " hours");
	}

	strcpy(s[9], "Game quits: ");
	if (stat.GameQuit == 1) strcat(s[9], "a time");
	else
	{
		ltoa(stat.GameQuit, ia, 10);
		strcat(s[9], ia);
		strcat(s[9], " times");
	}

	strcpy(s[10], "All deaths: ");
	if (stat.AllDeath == 1) strcat(s[10], "a time");
	else
	{
		ltoa(stat.AllDeath, ia, 10);
		strcat(s[10], ia);
		strcat(s[10], " times");
	}

	strcpy(s[11], "Deaths from the wall: ");
	if (stat.WallDeath == 1) strcat(s[11], "a time");
	else
	{
		ltoa(stat.WallDeath, ia, 10);
		strcat(s[11], ia);
		strcat(s[11], " times");
	}

	strcpy(s[12], "Deaths from the self-crossing: ");
	if (stat.SCrossingDeath == 1) strcat(s[12], "a time");
	else
	{
		ltoa(stat.SCrossingDeath, ia, 10);
		strcat(s[12], ia);
		strcat(s[12], " times");
	}

	DrawWindow(str, size);

	do
	{
		ChooseY(com, 12, TRUE);
	} while (!(com.back || com.esc));

	com.choose = 4;
	com.f = TRUE;
}

// ëó÷øèå ðåçóëüòàòû èãðîêîâ
void Leaderboard(command &com)
{
	com.choose = 5;
	com.f = TRUE;
}

// ÑÎÇÄÀÒÅËÜ
void Creator(command &com)
{
	com.choose = 1;
	const unsigned short size = 21 + 9;
	char creator[size][81] =
	{ // CREATOR
		"|                                  Creator:                                   |\n",
		"|                             Melnichuk Vladislav                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                Company logo:                                |\n",
		"|                     This place reserve for company logo.                    |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                Logo with game is located in the same folder.                |\n",
		"|                                                                             |\n",
		"|                                     :)                                      |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                                                                             |\n"
	};

	do
	{
		com.OldChoose = com.esc = com.back = com.enter = FALSE;

		// Âûâîä íà÷àëüíûõ ñòðîê
		system("cls"); WallX(); cout << '\n'; WallY(); Space(77); WallY(); cout << '\n';

		// Âûâîä èíôîðìàöèè î ñîçäàòåëå è êîìïàíèè
		for (int i = com.choose - 1; i < 20 + com.choose; i++) cout << creator[i];

		// Âûâîä êîíå÷íûõ ñòðîê
		WallY(); Space(77); WallY(); cout << '\n'; WallX();

		do
		{
			ChooseY(com, 10, TRUE);
		} while (!(com.back || com.OldChoose || com.esc));
	} while (!(com.back || com.esc));

	com.choose = 6;
	com.f = TRUE;
}

// ïîìîùü
void Help(command &com)
{
	com.choose = 1;
	const unsigned short size = 21 + 9;
	char help[size][81] =
	{ // HELP
		"|                               Control buttons:                              |\n",
		"|   -----------------------------------------------------------------------   |\n",
		"|                                                                             |\n",
		"|                               Menu selection:                               |\n",
		"|                                                                             |\n",
		"|          /\\      ||                                                         |\n",
		"|   Press  ||  or  \\/  for switch between fields (in any menu).               |\n",
		"|                                                                             |\n",
		"|   Press  <-- (also BACKSPACE)  or  --> (also ENTER)                         |\n",
		"|   for switch between game menu or setting field.                            |\n",
		"|                                                                             |\n",
		"|   Press  H (h)  in any place of game to go in Help menu.                    |\n",
		"|                                                                             |\n",
		"|   Press ESC to go back in main menu.                                        |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                               Options selection:                            |\n",
		"|                                                                             |\n",
		"|   Also as in the other menu, except: <--, -->, BACKSPACE and ENTER.         |\n",
		"|                                                                             |\n",
		"|   Press  <--  or  -->  for switch between fields.                           |\n",
		"|                                                                             |\n",
		"|   Press BACKSPACE to exit the options menu.                                |\n",
		"|                                                                             |\n",
		"|   Press ENTER to confirm the selection.                                     |\n",
		"|                                                                             |\n",
		"|                                                                             |\n",
		"|                               Control in game:                              |\n",
		"|                                                                             |\n",
		"|                                                                             |\n"
	};

	do
	{
		com.OldChoose = com.esc = com.back = com.enter = FALSE;

		// Âûâîä íà÷àëüíûõ ñòðîê
		system("cls"); WallX(); cout << '\n'; WallY(); Space(77); WallY(2); Space(36);
		cout << "Help:";
		Space(36); WallY(2); Space(77); WallY(); cout << '\n';

		// Âûâîä ïîìîùè
		for (int i = com.choose - 1; i < 18 + com.choose; i++) cout << help[i];

		// Âûâîä êîíå÷íûõ ñòðîê
		WallY(); Space(77); WallY(); cout << '\n'; WallX();

		do
		{
			ChooseY(com, 12, TRUE);
		} while (!(com.back || com.OldChoose || com.esc));
	} while (!(com.back || com.esc));

	com.choose = 7;
	com.f = TRUE;
}


/////////////////////////////////////////////////
//       ÔÓÍÊÖÈÈ ÂÛÁÎÐÀ Â ÐÀÇËÈ×ÍÛÕ ÌÅÍÞ       //
/////////////////////////////////////////////////

// ôóíêöèÿ âåðòèêàëüíîãî âûáîðà â ðàçëè÷íûõ ìåíþ
void ChooseY(command &com, const unsigned short &num, const bool &h)
{
	short key = getch();

	if ((key == 72 || key == 104) && !h) Help(com); // ïðîâåðêà íà âõîä â Help

	// ïðîâåðêà íà íàæàòèå îïðåäåë¸ííûõ êëàâèø
	else if (key == 8) com.back = TRUE;
	else if (key == 13) com.enter = TRUE;
	else if (key == 27) com.esc = TRUE;

	else if (key == 224)
	{
		key = getch();

		switch (key)
		{
			// Âûáîð
			case 72:
				if (com.choose == 1)
				{
					if (!h)
					{
						com.choose = num;
						com.OldChoose = TRUE;
					}
				}
				else
				{
					com.choose--;
					com.OldChoose = TRUE;
				}
				break;
			case 80:
				if (com.choose == num)
				{
					if (!h)
					{
						com.choose = 1;
						com.OldChoose = TRUE;
					}
				}
				else
				{
					com.choose++;
					com.OldChoose = TRUE;
				}
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
void ChooseX(command &com, const unsigned short &num, const unsigned short &chooseX)
{
	short key = getch();

	if (key == 72 || key == 104) Help(com); // ïðîâåðêà íà âõîä â Help

	// // ïðîâåðêà íà íàæàòèå îïðåäåë¸ííûõ êëàâèø
	else if (key == 8) com.back = TRUE;
	else if (key == 13) com.enter = TRUE;
	else if (key == 27) com.esc = TRUE;

	else if (key == 224)
	{
		key = getch();

		switch (key)
		{
			// Âûáîð
			case 75:
				if (com.choose % chooseX == 1)
				{
					com.choose = com.choose + chooseX - 1;
					com.OldChoose = TRUE;
				}
				else
				{
					com.choose--;
					com.OldChoose = TRUE;
				}
				break;
			case 77:
				if (!(com.choose % chooseX))
				{
					com.choose = com.choose - chooseX + 1;
					com.OldChoose = TRUE;
				}
				else
				{
					com.choose++;
					com.OldChoose = TRUE;
				}
				break;
			case 72:
				if ((com.choose - chooseX) <= 0) com.choose = num - chooseX + 1;
				else if (com.choose % chooseX == 1) com.choose = com.choose - chooseX;
				else com.choose = (com.choose - chooseX - 1) / chooseX * chooseX + 1;
				com.OldChoose = TRUE;
				break;
			case 80:
				com.choose = (com.choose + chooseX - 1) / chooseX * chooseX + 1;
				if (com.choose > num) com.choose = 1;
				com.OldChoose = TRUE;
		}
	}
}


/////////////////////////////////////////////////
//          ÔÓÍÊÖÈß ÐÈÑÎÂÀÍÈß ÎÊÍÀ             //
/////////////////////////////////////////////////

void DrawWindow(char **str, const unsigned short &size, const unsigned short &BottomSize)
{
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



//////////////////////////////////////////////////////////////////////////////////////////////
//                                 ÔÓÍÊÖÈÈ ÈÃÐÛ                                             //
//////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
//             ÎÁÙÈÅ ÔÓÍÊÖÈÈ ÈÃÐ               //
/////////////////////////////////////////////////

// void Input();

// void Eat();




/////////////////////////////////////////////////////////////////////////////////////
//                             ÐÅÆÈÌ CLASSIC 80S                                   //
/////////////////////////////////////////////////////////////////////////////////////

void Classic_80s(options &opt)
{

}


/////////////////////////////////////////////////////////////////////////////////////
//                            ÐÅÆÈÌ CLASSIC 90S                                    //
/////////////////////////////////////////////////////////////////////////////////////
void Classic_90s(options &opt)
{
	system("cls");

	cout << "+-----------------------------------------------------------------------------+\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                      :)                                     |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                              :)                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "+-----------------------------------------------------------------------------+";

	Sleep(5000);
}


/////////////////////////////////////////////////////////////////////////////////////
//                            ÐÅÆÈÌ LEVELS MODE                                    //
/////////////////////////////////////////////////////////////////////////////////////

void Level_mode(options &opt)
{
	system("cls");

	cout << "+-----------------------------------------------------------------------------+\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                      :)                                     |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                   :)                                                        |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                     :)                      |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "|                                                                             |\n";
	cout << "+-----------------------------------------------------------------------------+";

	Sleep(7000);
}


/////////////////////////////////////////////////////////////////////////////////////
//                            ÐÅÆÈÌ ESOFT MODE                                     //
/////////////////////////////////////////////////////////////////////////////////////

void Esoft_mode(options &opt)
{

}


/////////////////////////////////////////////////////////////////////////////////////
//                            ÐÅÆÈÌ CUSTOM MODE                                    //
/////////////////////////////////////////////////////////////////////////////////////

void Custom_mode(options &opt)
{
	system("cls");

	int l;
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << ":)";

			srand(time(NULL));
			l = rand() % 9 + 1;
			for (int i = 0; i < l; i++) cout << ' ';

			l = rand() % 500 + 1;
			Sleep(l);
		}
		if (i != 23) cout << '\n';
	}

	Sleep(15000);
}
