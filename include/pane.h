#pragma once
#include"debug_option.h"
#include<string>
#include<vector>
struct TerminalChar{
	wchar_t ch;
	uint8_t frontcolor;
	uint8_t backgroundcolor;
};
struct Pane{
	Pane(int rows,int cols);
	~Pane();
	int fd()const;
	void write_input(const char* data,size_t len);
	void read_output();
	void resize(int rows,int cols);
	void set_rect(int x,int y,int w,int h);
	void render();
	bool alive()const;
PRIVATE_ON_RELEASE:
	int pty_fd_=-1;
	pid_t child_pid_=-1;
	int rows_,cols_;
	int x_=0,y_=0,w_=0,h_=0;
	std::vector<TerminalChar> screen_;
};
