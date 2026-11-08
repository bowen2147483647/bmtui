#include"pane.h"
#include<pty.h>
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
#include<cerrno>
Pane::Pane(int rows,int cols):rows_(rows),cols_(cols){
	screen_.resize(rows*cols);
	struct winsize ws;
	ws.ws_row=rows;
	ws.ws_col=cols;
	child_pid_=forkpty(&pty_fd_,nullptr,nullptr,&ws);
	if(child_pid_==0){
		execl("/bin/sh","sh",nullptr);
		_exit(1);
	}
	if(child_pid_<0){
		//TODO: DO SOMETHING
	}
}
Pane::~Pane(){
	if(pty_fd_>=0){
		close(pty_fd_);
		pty_fd_=-1;
	}
	if(child_pid_>0){
		waitpid(child_pid_,nullptr,0);
		child_pid_=-1;
	}
}
int Pane::fd()const{
	return pty_fd_;
}
