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
int symbol_colors[] = { 10, 14, 15, 13, 11, 12, 15, 15 };

// The co-ordinates of the center of wheel 1
int wheels_x = 10;
int wheels_y = 5;
// The wheel rotations range from 0 to SYMBOL_COUNT-1
int wheel_rotations[WHEEL_COUNT] = { 0, 0, 0 };
int wheel_symbols[WHEEL_COUNT][SYMBOL_COUNT] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 4, 6, 2, 3, 1, 0, 7, 5 },
	{ 7, 3, 5, 4, 0, 2, 1, 6 }
};


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
void Clear_Screen(void)
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


void Draw_Machine()
{
	Set_Color(default_color, 0);
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
		int position = wheel_rotations[wheel_index] + i;
		position += SYMBOL_COUNT;
		position = position % SYMBOL_COUNT;
		int symbol = wheel_symbols[wheel_index][position];

		Set_Color(0, symbol_colors[symbol]);
		Draw_String(x_position - 1, wheels_y + i, " ");
		Draw_Char(x_position, wheels_y + i, symbol_chars[symbol]);
		Draw_String(x_position + 1, wheels_y + i, " ");
	}
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

	char key; // player input data

	int stopped_wheels = 0;

	while (true)
	{
		// SECTION: get player input
		if (_kbhit())
		{
			// get keyboard data, and filter it
			key = toupper(_getch());

			// if space key pressed, stop a wheel
			if (key == 32)
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

	//Clear_Screen();
	return 0;
}