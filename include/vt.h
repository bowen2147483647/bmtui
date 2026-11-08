#pragma once
#include"screen.h"
#include<cstddef>
#include<cstdint>
#include<string>
struct VtParser{
	//TODO: ESC ANSI STRING
	explicit VtParser(Screen& screen);
	void feed(const char* data,std::size_t len);
PRIVATE_ON_RELEASE:
	enum struct State{
		Ground,
		Escape,
		Csi,
		Charset,
		Osc
	};
	void ground_byte(unsigned char c);
	void escape_byte(unsigned char c);
	void csi_byte(unsigned char c);
	void dispatch(char final_char);
	void emit(char32_t cp);
	Screen& screen_;
	State state_=State::Ground;
	std::string params_;
	std::uint32_t utf8_acc_=0;
	int utf8_need_=0;
};
