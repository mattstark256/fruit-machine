#include "pch.h"
#include "Wheels.h"


Wheels::Wheels()
{
}


void Wheels::Spin_Wheel(int wheel_index)
{
	wheel_rotations[wheel_index]++;
	if (wheel_rotations[wheel_index] == SYMBOL_COUNT) wheel_rotations[wheel_index] = 0;
}


void Wheels::Draw_Wheel(int wheel_index)
{
	int x_position = wheels_x + wheel_index * 4;

	for (int i = -2; i <= 2; i++)
	{
		int rotation = wheel_rotations[wheel_index] + i;
		rotation += SYMBOL_COUNT;
		rotation = rotation % SYMBOL_COUNT;
		int symbol = wheel_symbols[wheel_index][rotation];

		display->Set_Color(0, symbol_colors[symbol]);
		display->Draw_String(x_position - 1, wheels_y + i, " ");
		display->Draw_Char(x_position, wheels_y + i, symbol_chars[symbol]);
		display->Draw_String(x_position + 1, wheels_y + i, " ");
	}
}


// This currently only works with 3 wheels
int Wheels::Get_Matched_Count()
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


void Wheels::Set_Display(Display* display)
{
	this->display = display;
}


void Wheels::Initialize_Wheels()
{
	display->Use_Default_Color();

	for (int i = 0; i < WHEEL_COUNT + 1; i++)
	{
		display->Draw_String(wheels_x - 2 + i * 4, wheels_y, "-");
	}

	for (int i = 0; i < WHEEL_COUNT; i++)
	{
		Draw_Wheel(i);
	}
}