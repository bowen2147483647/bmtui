#include<sys/ioctl.h>
#include<unistd.h>
#include<termios.h>
struct winsize get_terminal_size(){
	struct winsize ws{};
	if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&ws)<0||ws.ws_row==0||ws.ws_col==0){
		ws.ws_row=24;
		ws.ws_col=80;
	}
	return ws;
}
