#include<cstdio>
#include<unistd.h>
void enter_fullscreen(){
	write(STDOUT_FILENO,"\033[?1049h\033[2J\033[H\033[?25l",8+4+3+6);
}
void leave_fullscreen(){
	write(STDOUT_FILENO,"\033[?25h\033[?1049l",6+8);
}
