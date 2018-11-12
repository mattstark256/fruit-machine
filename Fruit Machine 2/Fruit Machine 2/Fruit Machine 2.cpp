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

// I'm surprised the La Mothe code doesn't do this:
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25


// PROTOTYPES



// GLOBALS

CONSOLE_SCREEN_BUFFER_INFO con_info; // holds screen info
HANDLE hconsole; // handle to console


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

	// print the string in current color
	//printf("%s", s);
	cout << s;
}


// this function sets the color of the console output
void Set_Color(int fcolor, int bcolor = 0)
{
	// 4 bits for foreground and 4 for background
	// 1st bit is intensity, the rest are RGB
	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) |
		fcolor));
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


// MAIN GAME LOOP

int main()
{
	init_Graphics();

	Set_Color(15, 0);
	Draw_String(10, 5, "I'm at 10,5");

	//Clear_Screen();

	return 0;

}

// Greyscale palette
// 0 (0000) - black
// 8 (1000) - dark grey (black + intense)
// 7 (0111) - light grey
// 15 (1111) - white (white + intense)