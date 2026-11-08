#include"debug_option.h"
#include"pane.h"
#include"ansi_operation.h"
#include"getsize.h"
#include<termios.h>
#include<unistd.h>
#include<sys/select.h>
void enter_raw_mode(struct termios& orig){
	struct termios raw;
	tcgetattr(STDIN_FILENO,&orig);
	raw=orig;
	raw.c_lflag&=~(ECHO|ICANON|ISIG);
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}
void exit_raw_mode(struct termios orig){
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig);
}
int main(int argc,char** argv){
	IGNORE(argc);
	IGNORE(argv);
	struct winsize ws=get_terminal_size();
	Pane pDefault(ws.ws_row,ws.ws_col);
	struct termios orig;
	enter_raw_mode(orig);
	enter_fullscreen();
	char buf[4096];
	while(true){
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO,&fds);
		FD_SET(pDefault.fd(),&fds);
		select(pDefault.fd()+1,&fds,nullptr,nullptr,nullptr);
		if(FD_ISSET(STDIN_FILENO,&fds)){
			int n=read(STDIN_FILENO,buf,sizeof(buf));
			if(n<=0)break;
			IGNORE_VALUE(write(pDefault.fd(),buf,n));
		}
		if(FD_ISSET(pDefault.fd(),&fds)){
			int n=read(pDefault.fd(),buf,sizeof(buf));
			if(n<=0)break;
			IGNORE_VALUE(write(STDOUT_FILENO,buf,n));
		}
	}
	leave_fullscreen();
	exit_raw_mode(orig);
	return 0;
}
