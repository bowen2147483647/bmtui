#pragma once
#include"screen.h"
#include"debug_option.h"
#include<string>
struct Renderer{
	explicit Renderer(int out_fd);
	void draw(const Screen& s);
PRIVATE_ON_RELEASE:
	int out_fd_=1;
	std::string buf_;
};
