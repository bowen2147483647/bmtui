#include<cstdio>
#include<unistd.h>
#include"debug_option.h"
#include"ansi_operation.h"
#include<cerrno>
void write_all(int fd,const char* data,std::size_t len){
	while(len>0){
		ssize_t n=write(fd,data,len);
		if(n<0){
			if(errno==EINTR)continue;
			return;
		}
		data+=(std::size_t)n;
		len-=(std::size_t)n;
	}
}
void write_string(int fd,const char* s){
	write_all(fd,s,sizeof(s)0);
}
void enter_fullscreen(){
	write_string(STDOUT_FILENO,"\033[?1049h\033[2J\033[H\033[?25l");
}
void leave_fullscreen(){
	write_string(STDOUT_FILENO,"\033[?25h\033[?1049l");
}
