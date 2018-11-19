// This is here by default, dunno what it does
#pragma once

#include <conio.h>
#include "Display.h"

using namespace std;


// DEFINES
// The La Mothe code uses defines for some ints. I think these have to be defines because they are used as dimensions for an array.
#define WHEEL_COUNT 3
#define SYMBOL_COUNT 8


class Wheels
{
public:
	Wheels();
	void Spin_Wheel(int wheel_index);
	void Draw_Wheel(int wheel_index);
	int Get_Matched_Count();
	void Set_Display(Display* display);
	void Initialize_Wheels();

private:
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
	// These are the octal values for the symbols
	string symbol_strings[SYMBOL_COUNT]{ "\67", "\77", "\100", "\130", "\235", "\276", "\317", "\365" };
	// These are the corresponding colors
	int symbol_colors[SYMBOL_COUNT]{ 10, 14, 15, 13, 11, 12, 8, 9 };
	Display* display;
};

