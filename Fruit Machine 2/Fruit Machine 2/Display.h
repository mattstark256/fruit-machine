// This is here by default, dunno what it does
#pragma once

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;


// DEFINES
// The La Mothe code uses defines for some ints. I think these have to be defines because they are used as dimensions for an array.
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25


class Display
{
public:
	Display();

	void Init_Graphics();
	void Draw_String(int x, int y, string s);
	void Set_Color(int fcolor, int bcolor = 0);
	void Clear_Screen();
	void Clear_Line(int line);
	void Print_All_Chars();
	void Use_Default_Color();

private:
	CONSOLE_SCREEN_BUFFER_INFO con_info; // holds screen info
	HANDLE hconsole; // handle to console
	int default_color_f = 15;
	int default_color_b = 0;
};

