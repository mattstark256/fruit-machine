// This is here by default on C++ console applications in Visual Studio.
#include "pch.h"

// These are all the includes from the La Mothe code. I'm not sure what they're all used for.
//#include <stdio.h>
//#include <stdlib.h>
#include <ctype.h> 
//#include <conio.h>
//#include <windows.h>
//#include <time.h>
//#include <string>
//#include <iostream>

#include "Display.h"
#include "Wheels.h"

using namespace std;

// GLOBALS

Display display;
Wheels wheels;

int coins = 30;

// This is used to adjust the position of all the text below the fruit machine.
int text_x = 2;
int text_y = 11;


// FUNCTIONS

void Draw_Title()
{
	display.Use_Default_Color();
	display.Draw_String(4, 1, "O N E   A R M E D   B A N D I T");
}


void Update_Coins_Text()
{
	display.Use_Default_Color();
	display.Draw_String(23, 6, "Coins: " + to_string(coins) + "    ");
}


// Return the last input
char Get_Input()
{
	char key = 0;
	if (_kbhit()) { key = toupper(_getch()); }
	return key;
}


// MAIN GAME LOOP

int main()
{
	display.Init_Graphics();
	wheels.Set_Display(&display);

	Draw_Title();
	wheels.Initialize_Wheels();
	Update_Coins_Text();



	display.Draw_String(text_x, text_y, "Match symbols to earn coins!");
	display.Draw_String(text_x, text_y + 2, "Press space to play (costs 5 coins).");

	bool awaiting_input = true;
	while (awaiting_input)
	{
		char key = Get_Input();
		if (key == 32) // Key 32 is space key
		{
			awaiting_input = false;
		}
	}

	display.Clear_Line(text_y);
	display.Clear_Line(text_y + 2);



	bool playing_game = true;
	while (playing_game)
	{
		coins -= 5;
		Update_Coins_Text();

		display.Draw_String(text_x, text_y, "Press space to stop each wheel.");

		int stopped_wheels = 0;
		while (stopped_wheels < WHEEL_COUNT)
		{
			char key = Get_Input();
			if (key == 32) // Key 32 is space key
			{
				stopped_wheels++;
			}

			for (int i = stopped_wheels; i < WHEEL_COUNT; i++)
			{
				wheels.Spin_Wheel(i);
				wheels.Draw_Wheel(i);
			}

			Sleep(100);
		}

		display.Clear_Line(text_y);



		display.Use_Default_Color();
		int matched_count = wheels.Get_Matched_Count();
		if (matched_count == 0)
		{
			display.Draw_String(text_x, text_y, "You didn't match any.");
		}
		if (matched_count == 2)
		{
			display.Draw_String(text_x, text_y, "You matched 2! You win 10 coins!");
			coins += 10;
			Update_Coins_Text();
		}
		if (matched_count == 3)
		{
			display.Draw_String(text_x, text_y, "You matched 3! You win 20 coins!");
			coins += 20;
			Update_Coins_Text();
		}

		if (coins < 5)
		{
			display.Draw_String(text_x, text_y + 2, "You're out of money. Get out of here!");
		}
		else
		{
			display.Draw_String(text_x, text_y + 2, "Press space to play again (costs 5 coins).");
		}

		display.Draw_String(text_x, text_y + 3, "Press Q to quit.");

		bool awaiting_input = true;
		while (awaiting_input)
		{
			char key = Get_Input();
			if (coins >= 5 && key == 32) // Key 32 is space key
			{
				awaiting_input = false;
			}
			else if (key == 'Q')
			{
				awaiting_input = false;
				playing_game = false;
			}
		}

		display.Clear_Line(11);
		display.Clear_Line(13);
		display.Clear_Line(14);
	}

	display.Draw_String(text_x, text_y, "Bye! Come again soon!");

	// Move the cursor so the quit text appears in the right place
	display.Draw_String(0, text_y + 4, "");

	return 0;
}