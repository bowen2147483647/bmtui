#include"renderer.h"
#include<cstddef>
#include<unistd.h>
void append_uint(std::string& out,unsigned v){
	char digits[12];
	int n=0;
	do{
		digits[n++]=(char)('0'+v%10);
		v/=10;
	}while(v>0);
	while(n>0)out+=digits[--n];
}
void append_utf8(std::string& out,wchar_t wc){
	char32_t cp=(char32_t)wc;
	if(cp<0x80){
		out+=(char)(cp);
	}
	else if(cp<0x800){
		out+=(char)(0xC0|(cp>>6));
		out+=(char)(0x80|(cp&0x3F));
	}
	else if(cp<0x10000){
		out+=(char)(0xE0|(cp>>12));
		out+=(char)(0x80|((cp>>6)&0x3F));
		out+=(char)(0x80|(cp&0x3F));
	}
	else{
		out+=(char)(0xF0|(cp>>18));
		out+=(char)(0x80|((cp>>12)&0x3F));
		out+=(char)(0x80|((cp>>6)&0x3F));
		out+=(char)(0x80|(cp&0x3f));
	}
}
void append_cup(std::string& out,int row,int col){
	out+="\033[";
	append_uint(out,(unsigned)(row+1));
	out+=";";
	append_uint(out,(unsigned)(col+1));
	out+="H";
}
Renderer::Renderer(int out_fd):out_fd_(out_fd){
}
void Renderer::draw(const Screen& s){
	buf_.clear();
	buf_.append("\033[?25l");
	for(int r=0;r<s.rows();++r){
		append_cup(buf_,r,0);
		int last=-1;
		for(int c=s.cols()-1;c>=0;c--){
			if(s.at(r,c).ch!=L' '){
				last=c;
				break;
			}
		}
		for(int c=0;c<=last;c++)append_utf8(buf_,s.at(r,c).ch);
		buf_.append("\033[K");
	}
	append_cup(buf_,s.cursor_row(),s.cursor_col());
	buf_.append("\033[?25h");
	IGNORE_VALUE(write(out_fd_,buf_.data(),buf_.size()));
}
