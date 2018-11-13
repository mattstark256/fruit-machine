#include "pch.h"
#include "Display.h"


Display::Display()
{
}


void Display::Init_Graphics()
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
void Display::Draw_String(int x, int y, string s)
{
	COORD cursor_pos;
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);

	cout << s;
}


// This can be used to print characters above the 0-127 range.
void Display::Draw_Char(int x, int y, int charID)
{
	COORD cursor_pos;
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);

	char c = charID;
	cout << c;
}


// this function sets the color of the console output
void Display::Set_Color(int fcolor, int bcolor)
{
	// 4 bits for foreground and 4 for background
	// 1st bit is intensity, the rest are RGB
	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) | fcolor));
}


// This function clears the screen
void Display::Clear_Screen()
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


void Display::Clear_Line(int line)
{
	Set_Color(15, 0);

	for (int x = 0; x < SCREEN_WIDTH; x++)
		Draw_String(x, line, " ");
}


// This can be used to view the available characters. It's not part of the game.
void Display::Print_All_Chars()
{
	for (int y = 0, i = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++, i++) {
			Draw_Char(x, y, i);
			if (i > 256) return;
		}
	}
}


void Display::Use_Default_Color()
{
	Set_Color(default_color_f, default_color_b);
}