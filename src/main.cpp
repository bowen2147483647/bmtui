#include"debug_option.h"
#include"pane.h"
#include"ansi_operation.h"
#include"getsize.h"
#include"screen.h"
#include"vt.h"
#include"renderer.h"
#include<cstddef>
#include<termios.h>
#include<unistd.h>
#include<sys/select.h>
#include<signal.h>
const char kPrefixKey=0x02;
int g_resized,g_prefix;
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
void on_winch(int sig){
	IGNORE(sig);
	g_resized=1;
}
int main(int argc,char** argv){
	IGNORE(argc);
	IGNORE(argv);
	struct winsize ws=get_terminal_size();
	Pane pDefault(ws.ws_row,ws.ws_col);
	Screen screen(ws.ws_row,ws.ws_col);
	VtParser vt(screen);
	Renderer renderer(STDOUT_FILENO);
	signal(SIGWINCH,on_winch);
	struct termios orig;
	enter_raw_mode(orig);
	enter_fullscreen();
	renderer.draw(screen);
	char buf[4096];
	bool dirty=false;
	bool quit=0;
	while(true){
		if(g_resized){
			g_resized=0;
			ws=get_terminal_size();
			screen.resize(ws.ws_row,ws.ws_col);
			pDefault.resize(ws.ws_row,ws.ws_col);
			renderer.draw(screen);
		}
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO,&fds);
		FD_SET(pDefault.fd(),&fds);
		select(pDefault.fd()+1,&fds,nullptr,nullptr,nullptr);
		if(FD_ISSET(STDIN_FILENO,&fds)){
			ssize_t n=read(STDIN_FILENO,buf,sizeof(buf));
			if(n<=0)break;
			char out[4096];
			std::size_t out_len=0;
			for(ssize_t i=0;i<n;i++){
				char c=buf[i];
				if(g_prefix){
					g_prefix=false;
					if(c=='q'){
						quit=1;
						break;
					}
					if(c==kPrefixKey){
						out[out_len++]=c;
						continue;
					}
				}
				if(c==kPrefixKey){
					g_prefix=true;
					continue;
				}
				out[out_len++]=c;
			}
			if(out_len>0)write_all(pDefault.fd(),out,out_len);
		}
		if(!quit&&FD_ISSET(pDefault.fd(),&fds)){
			int n=read(pDefault.fd(),buf,sizeof(buf));
			if(n<=0)break;
			vt.feed(buf,(std::size_t)n);
			dirty=true;
		}
		if(dirty){
			renderer.draw(screen);
			dirty=false;
		}
	}
	leave_fullscreen();
	exit_raw_mode(orig);
	return 0;
}
