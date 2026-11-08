#include"vt.h"
VtParser::VtParser(Screen& screen):screen_(screen){
}
void VtParser::feed(const char* data,std::size_t len){
	for(std::size_t i=0;i<len;i++){
		const unsigned char c=(unsigned char)data[i];
		switch(state_){
			case State::Ground:{
				ground_byte(c);
				break;
			}
			case State::Escape:{
				escape_byte(c);
				break;
			}
			case State::Csi:{
				csi_byte(c);
				break;
			}
			case State::Charset:{
				state_=State::Ground;
				break;
			}
			case State::Osc:{
				if(c==0x07)state_=State::Ground;
				else if(c==0x1B)state_=State::Escape;
				break;
			}
		}
	}
}
void VtParser::ground_byte(unsigned char c){
	if(utf8_need_>0){
		if((c&0xC0)==0x80){
			utf8_acc_=(utf8_acc_<<6)|(c&0x3F);
			if(--utf8_need_==0)emit((char32_t)(utf8_acc_));
			return;
		}
		utf8_need_=0;
	}
	if(c==0x1B){
		state_=State::Escape;
		return;
	}
	if(c==0x0A){
		screen_.newline();
		return;
	}
	if(c==0x0D){
		screen_.carriage_return();
		return;
	}
	if(c==0x08){
		screen_.backspace();
		return;
	}
	if(c==0x09){
		screen_.tab();
		return;
	}
	if(c<0x20){
		//TODO: ADD MORE CONTROL CHARS
		return;
	}
	// UTF-8 First Char
	if(c<0x80){
		emit((char32_t)(c));
	}
	else if((c&0xE0)==0xC0){
		utf8_acc_=c&0x1F;
		utf8_need_=1;
	}
	else if((c&0xF0)==0xE0){
		utf8_acc_=c&0x0F;
		utf8_need_=2;
	}
	else if((c&0xF8)==0xF0){
		utf8_acc_=c&0x07;
		utf8_need_=3;
	}
	else{
		emit((char32_t)('?'));
		// INVALID First Char
	}
}
void VtParser::escape_byte(unsigned char c){
	if(c=='['){
		params_.clear();
		state_=State::Csi;
	}
	else if(c=='('||c==')'||c=='#'||c=='%'||c=='*'||c=='+'){
		state_=State::Charset;
	}
	else if(c==']'||c=='_'||c=='^'||c=='X'){
		state_=State::Osc;
	}
	else{
		state_=State::Ground;
	}
}
void VtParser::csi_byte(unsigned char c){
	if(c>=0x20&&c<=0x3F){
		if(params_.size()<64)params_.push_back((char)c);
	}
	else if(c>=0x40&&c<=0x7E){
		dispatch((char)c);
		state_=State::Ground;
	}
	else{
		//THE ESCAPE IS INVALID
		state_=State::Ground;
	}
}
void VtParser::dispatch(char final_char){
	//TODO: ESCAPE
	IGNORE(final_char);
}
void VtParser::emit(char32_t cp){
	screen_.put((wchar_t)cp);
}
