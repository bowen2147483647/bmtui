#include<sys/ioctl.h>
#include<unistd.h>
#include<termios.h>
struct winsize get_terminal_size(){
	struct winsize ws{};
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&ws);
	return ws;
}
