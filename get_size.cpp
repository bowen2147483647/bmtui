#include<sys/ioctl.h>
struct winsize get_terminal_size(){
	struct winsize ws{};
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&ws);
	return ws;
}
