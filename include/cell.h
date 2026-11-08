#pragma once
#include<cstdint>
struct Cell{
	wchar_t ch=L' ';
	uint8_t fg_color=7;	//ANSI color
	uint8_t bg_color=0;
};
