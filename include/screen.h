#pragma once
#include"cell.h"
#include"debug_option.h"
#include<vector>
struct Screen{
	Screen(int rows,int cols);
	int rows()const;
	int cols()const;
	void resize(int rows,int cols);
	const Cell& at(int row,int col)const;
	int cursor_row()const;
	int cursor_col()const;
	void set_cursor(int row,int col);
	void put(wchar_t ch);
	void newline();
	void carriage_return();
	void backspace();
	void tab();
	void clear();
	void set_colors(uint8_t fg,uint8_t bg);
	void set_reverse(bool on);
	void set_bold(bool on);
PRIVATE_ON_RELEASE:
	Cell& cell_at(int row,int col);
	Cell& int_at(std::size_t row,std::size_t col);
	const Cell& int_at(std::size_t row,std::size_t col)const;
	void scroll_up();
	int rows_=0,cols_=0;
	int cursor_row_=0,cursor_col_=0;
	bool wrapped_=false;
	uint8_t pen_fg_=7,pen_bg_=0;
	bool pen_bold_=false;
	bool pen_reverse_=false;
	std::vector<Cell> cells_;
};
