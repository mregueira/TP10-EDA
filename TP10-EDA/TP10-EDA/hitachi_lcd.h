#pragma once
#include "basic_lcd.h"
#include <vector>
#include "windows.h"
#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#define ENABLE 0x01
#define RS_IR 0x00
#define RS_DR 0x02
#define LCD_CLEAR 0x01
#define LCD_CURSOR_MOVE 0x10
#define CURSOR_MOVE_L 0x00
#define CURSOR_MOVE_R 0x04
#define EMPTY_CHAR 0x20

using namespace std;

class hitachi_lcd : public basic_lcd 
{
public:
	hitachi_lcd();

	virtual bool lcdInitOk();
	//virtual FT_STATUS lcdGetError();
	virtual bool lcdClear(); 
	virtual bool lcdClearToEOL();
	virtual basic_lcd& operator<<(const unsigned char c);
	virtual basic_lcd& operator<<(const char * c);
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

	bool lcdScrollMsg(string Tit);
	void lcdPrintDate(vector<char>& Date);
	void lcdResetCharCount();

	~hitachi_lcd();

private:
	FT_HANDLE Handle;
	FT_STATUS lcd_stat;
	FT_STATUS write_nybble_to_ir(FT_HANDLE h, UCHAR data);
	FT_STATUS write_nybble_to_dr(FT_HANDLE h, UCHAR data);
	FT_STATUS write_byte_to_ir(FT_HANDLE h, UCHAR data);
	FT_STATUS write_byte_to_dr(FT_HANDLE h, UCHAR data);
	void clr_display(FT_HANDLE h);
	void init_4_bit_mode(FT_HANDLE h);
	virtual void lcdUpdateCursor() {}
	bool error_log;

	unsigned int char_count;
};