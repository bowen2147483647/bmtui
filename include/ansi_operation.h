#pragma once
void enter_fullscreen();
void leave_fullscreen();
void write_all(int fd,const char* data,std::size_t length);
void write_string(int fd,const char* s);
