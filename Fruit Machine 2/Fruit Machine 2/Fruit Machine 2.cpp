// INCLUDES

// This is here by default on C++ console applications in Visual Studio.
#include "pch.h"

// These are all the includes from the La Mothe code. I'm not sure what they're all used for.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
using namespace std;


// DEFINES

// The La Mothe code uses defines for some ints. I think these have to be defines because they are used as dimensions for an array.
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHEEL_COUNT 3
#define SYMBOL_COUNT 8


// PROTOTYPES



// GLOBALS

CONSOLE_SCREEN_BUFFER_INFO con_info; // holds screen info
HANDLE hconsole; // handle to console

int default_color = 15;

// These are the character IDs for the wheel's symbols
int symbol_chars[] = { 55, 63, 64, 88, 157, 190, 207, 245 };
// These are the corresponding colors
int symbol_colors[] = { 10, 14, 15, 13, 11, 12, 8, 9 };

// The co-ordinates of the center of wheel 1
int wheels_x = 7;
int wheels_y = 6;
// The wheel rotations range from 0 to SYMBOL_COUNT-1
int wheel_rotations[WHEEL_COUNT] = { 0, 0, 0 };
int wheel_symbols[WHEEL_COUNT][SYMBOL_COUNT] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 4, 6, 2, 3, 1, 0, 7, 5 },
	{ 7, 3, 5, 0, 4, 2, 1, 6 }
};

// This is used to adjust the position of all the text below the fruit machine.
int text_x = 2;
int text_y = 11;

int coins = 30;


// FUNCTIONS

void init_Graphics()
{
	// this function initializes the console graphics engine

	COORD console_size = { SCREEN_WIDTH, SCREEN_HEIGHT }; // size of console

	// open i/o channel to console screen
	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);

	// make sure we are in 80x25
	SetConsoleScreenBufferSize(hconsole, console_size);

	// get details for console screen                       
	GetConsoleScreenBufferInfo(hconsole, &con_info);
}


// This function draws a string starting at the given coordinate, overwriting whatever was there before.
void Draw_String(int x, int y, string s)
{
	COORD cursor_pos;
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);

	cout << s;
}


// This can be used to print characters above the 0-127 range.
void Draw_Char(int x, int y, int charID)
{
	COORD cursor_pos;
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);

	char c = charID;
	cout << c;
}


// this function sets the color of the console output
void Set_Color(int fcolor, int bcolor = 0)
{
	// 4 bits for foreground and 4 for background
	// 1st bit is intensity, the rest are RGB
	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) | fcolor));
}


// This function clears the screen
void Clear_Screen()
{
	// set color to white on black
	Set_Color(15, 0);

	// I think the La Mothe version relies on the screen scrolling when newlines are added at the bottom, but this doesn't seem to happen in all cases, including on my desktop and laptop
	/*for (int index = 0; index <= 25; index++)
		Draw_String(0, SCROLL_POS, "\n");*/

	for (int x = 0; x < SCREEN_WIDTH; x++)
		for (int y = 0; y < SCREEN_HEIGHT; y++)
			Draw_String(x, y, " ");
}


void Clear_Line(int line)
{
	Set_Color(15, 0);

	for (int x = 0; x < SCREEN_WIDTH; x++)
		Draw_String(x, line, " ");
}


// This can be used to view the available characters. It's not part of the game.
void Print_All_Chars()
{
	for (int y = 0, i = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++, i++) {
			Draw_Char(x, y, i);
			if (i > 256) return;
		}
	}
}


void Spin_Wheel(int wheel_index)
{
	wheel_rotations[wheel_index]++;
	if (wheel_rotations[wheel_index] == SYMBOL_COUNT) wheel_rotations[wheel_index] = 0;
}


void Use_Default_Color()
{
	Set_Color(default_color, 0);
}


void Draw_Machine()
{
	Use_Default_Color();

	Draw_String(4, 1, "O N E   A R M E D   B A N D I T");

	for (int i = 0; i < WHEEL_COUNT + 1; i++)
	{
		Draw_String(wheels_x - 2 + i * 4, wheels_y, "-");
	}
}


void Draw_Wheel(int wheel_index)
{
	int x_position = wheels_x + wheel_index * 4;

	for (int i = -2; i <= 2; i++)
	{
		int rotation = wheel_rotations[wheel_index] + i;
		rotation += SYMBOL_COUNT;
		rotation = rotation % SYMBOL_COUNT;
		int symbol = wheel_symbols[wheel_index][rotation];

		Set_Color(0, symbol_colors[symbol]);
		Draw_String(x_position - 1, wheels_y + i, " ");
		Draw_Char(x_position, wheels_y + i, symbol_chars[symbol]);
		Draw_String(x_position + 1, wheels_y + i, " ");
	}
}


// This currently only works with 3 wheels
int Get_Matched_Count()
{
	int i = 0;
	int symbol_1 = wheel_symbols[0][wheel_rotations[0]];
	int symbol_2 = wheel_symbols[1][wheel_rotations[1]];
	int symbol_3 = wheel_symbols[2][wheel_rotations[2]];
	if (symbol_1 == symbol_2 && symbol_2 == symbol_3)
		i = 3;
	else if (symbol_1 == symbol_2 || symbol_2 == symbol_3 || symbol_1 == symbol_3)
		i = 2;
	return i;
}


void Update_Coins_Text()
{
	Use_Default_Color();
	Draw_String(23, 6, "Coins: " + to_string(coins) + "    ");
}


// MAIN GAME LOOP

int main()
{
	//Print_All_Chars();

	init_Graphics();

	Draw_Machine();
	Draw_Wheel(0);
	Draw_Wheel(1);
	Draw_Wheel(2);
	Update_Coins_Text();

	char key; // player input data

	Draw_String(text_x, text_y, "Match symbols to earn coins!");
	Draw_String(text_x, text_y + 2, "Press space to play (costs 5 coins).");

	// Get input
	bool awaiting_input = true;
	while (awaiting_input)
	{
		if (_kbhit())
		{
			key = toupper(_getch());

			if (key == 32) // Key 32 is space key
			{
				awaiting_input = false;
			}
		}
	}

	Clear_Line(text_y);
	Clear_Line(text_y + 2);

	bool playing_game = true;
	while (playing_game)
	{
		coins -= 5;
		Update_Coins_Text();

		Draw_String(text_x, text_y, "Press space to stop each wheel.");

		int stopped_wheels = 0;
		while (stopped_wheels < WHEEL_COUNT)
		{
			if (_kbhit())
			{
				key = toupper(_getch());

				if (key == 32) // Key 32 is space key
				{
					stopped_wheels++;
				}
			}

			for (int i = stopped_wheels; i < WHEEL_COUNT; i++)
			{
				Spin_Wheel(i);
				Draw_Wheel(i);
			}

			Sleep(100);
		}

		Clear_Line(text_y);


		Use_Default_Color();
		int matched_count = Get_Matched_Count();
		if (matched_count == 0)
		{
			Draw_String(text_x, text_y, "You didn't match any.");
		}
		if (matched_count == 2)
		{
			Draw_String(text_x, text_y, "You matched 2! You win 10 coins!");
			coins += 10;
			Update_Coins_Text();
		}
		if (matched_count == 3)
		{
			Draw_String(text_x, text_y, "You matched 3! You win 20 coins!");
			coins += 20;
			Update_Coins_Text();
		}

		if (coins < 5)
		{
			Draw_String(text_x, text_y + 2, "You're out of money. Get out of here!");
		}
		else
		{
			Draw_String(text_x, text_y + 2, "Press space to play again (costs 5 coins).");
		}

		Draw_String(text_x, text_y + 3, "Press Q to quit.");


		// Get input
		bool awaiting_input = true;
		while (awaiting_input)
		{
			if (_kbhit())
			{
				key = toupper(_getch());

				if (coins >= 5 && key == 32) // Key 32 is space key
				{
					awaiting_input = false;
				}

				if (key == 'Q')
				{
					awaiting_input = false;
					playing_game = false;
				}
			}
		}

		Clear_Line(11);
		Clear_Line(13);
		Clear_Line(14);
	}

	Draw_String(text_x, text_y, "Bye! Come again soon!");

	// Move the cursor so the quit text appears in the right place
	Draw_String(0, text_y + 4, "");

	return 0;
}