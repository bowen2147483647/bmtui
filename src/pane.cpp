#include"pane.h"
#include<pty.h>
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
#include<cerrno>
void set_pty_size(int fd,int rows,int cols){
	if(fd<0)return;
	struct winsize ws{};
	ws.ws_row=(unsigned short)rows;
	ws.ws_col=(unsigned short)cols;
	IGNORE_VALUE(ioctl(fd,TIOCSWINSZ,&ws));
}
Pane::Pane(int rows,int cols):rows_(rows),cols_(cols){
	struct winsize ws{};
	ws.ws_row=(unsigned short)rows;
	ws.ws_col=(unsigned short)cols;
	child_pid_=forkpty(&pty_fd_,nullptr,nullptr,&ws);
	if(child_pid_==0){
		execl("/bin/sh","sh",nullptr);
		_exit(127);
	}
	if(child_pid_<0){
		pty_fd_=-1;
	}
}
Pane::~Pane(){
	if(pty_fd_>=0){
		close(pty_fd_);
		pty_fd_=-1;
	}
	if(child_pid_<=0)return;
	waitpid(child_pid_,nullptr,0);
	child_pid_=-1;
}
int Pane::fd()const{
	return pty_fd_;
}
void Pane::resize(int rows,int cols){
	IGNORE(rows);
	IGNORE(cols);
	//TODO: do something
}
