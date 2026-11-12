#pragma once
#include"debug_option.h"
#include<cstddef>
#include<sys/types.h>
struct Pane{
	Pane(int rows,int cols);
	~Pane();
	Pane(const Pane&)=delete;
	Pane& operator=(const Pane&)=delete;
	int fd()const;
	void resize(int rows,int cols);
	void set_rect(int x,int y,int w,int h);
	void render();
	bool alive()const;
PRIVATE_ON_RELEASE:
	int pty_fd_=-1;
	pid_t child_pid_=-1;
	int rows_=0,cols_=0;
	int x_=0,y_=0,w_=0,h_=0;
};
