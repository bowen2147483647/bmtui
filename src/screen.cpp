#include"screen.h"
#include<algorithm>
#include<cstddef>
int clamp_positive(int v){
	return v>0?v:1;
}
Screen::Screen(int rows,int cols):
	rows_(clamp_positive(rows)),
	cols_(clamp_positive(cols)),
	cells_((std::size_t)(rows_)*(std::size_t)(cols_)){
	}
int Screen::rows()const{
	return rows_;
}
int Screen::cols()const{
	return cols_;
}
void Screen::resize(int rows,int cols){
	rows_=clamp_positive(rows);
	cols_=clamp_positive(cols);
	cells_.assign((std::size_t)(rows_)*(std::size_t)(cols_),Cell{});
	if(cursor_row_>=rows_)cursor_row_=rows_-1;
	if(cursor_col_>=cols_)cursor_col_=cols_-1;
	wrapped_=false;
}
const Cell& Screen::at(int row,int col)const{
	static const Cell cBlank{};
	if(row<0||col<0||row>=rows_||col>=cols_)return cBlank;
	return int_at((std::size_t)row,(std::size_t)col);
}
Cell& Screen::int_at(std::size_t row,std::size_t col){
	return cells_[row*(std::size_t)cols_+col];
}
const Cell& Screen::int_at(std::size_t row,std::size_t col)const{
	return cells_[row*(std::size_t)cols_+col];
}
Cell& Screen::cell_at(int row,int col){
	row=std::clamp(row,0,rows_-1);
	col=std::clamp(col,0,cols_-1);
	return int_at(row,col);
}
int Screen::cursor_row()const{
	return cursor_row_;
}
int Screen::cursor_col()const{
	return cursor_col_;
}
void Screen::set_cursor(int row,int col){
	cursor_row_=std::clamp(row,0,rows_-1);
	cursor_col_=std::clamp(col,0,cols_-1);
	wrapped_=false;
}
void Screen::put(wchar_t ch){
	//TODO: multi-column char
	if(wrapped_){
		wrapped_=false;
		cursor_col_=0;
		newline();
	}
	cell_at(cursor_row_,cursor_col_).ch=ch;
	if(cursor_col_+1>=cols_){
		cursor_col_=cols_+1;
		wrapped_=true;
	}
	else{
		cursor_col_++;
	}
}
void Screen::newline(){
	wrapped_=false;
	if(cursor_row_+1>=rows_){
		scroll_up();
	}
	else{
		cursor_row_++;
	}
}
void Screen::carriage_return(){
	wrapped_=false;
	cursor_col_=0;
}
void Screen::backspace(){
	wrapped_=false;
	if(cursor_col_)cursor_col_--;
}
void Screen::tab(){
	wrapped_=false;
	const int kTabWidth=8;
	cursor_col_=(cursor_col_/kTabWidth+1)*kTabWidth;
	if(cursor_col_+1>=cols_){
		cursor_col_=cols_+1;
		wrapped_=true;
	}
}
void Screen::clear(){
	std::fill(cells_.begin(),cells_.end(),Cell{});
	cursor_row_=0;
	cursor_col_=0;
	wrapped_=false;
}
void Screen::scroll_up(){
	cells_.erase(cells_.begin(),cells_.begin()+
			(std::ptrdiff_t)(std::size_t)cols_);
	cells_.resize((std::size_t)rows_*(std::size_t)cols_,Cell{});
}

