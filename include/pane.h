#pragma once
#include"debug_option.h"
#include"cell.h"
#include<string>
#include<vector>
#include<cstdint>
#include<cstddef>
#include<sys/types.h>
struct Pane{
	Pane(int rows,int cols);
	~Pane();
	int fd()const;
	void write_input(const char* data,size_t len);
	bool read_output(); //False means sub process exit
	void resize(int rows,int cols);
	void set_rect(int x,int y,int w,int h);
	void render();
	bool alive()const;
PRIVATE_ON_RELEASE:
	int pty_fd_=-1;
	pid_t child_pid_=-1;
	int rows_=0,cols_=0;
	int x_=0,y_=0,w_=0,h_=0;
	std::vector<Cell> screen_;
};
