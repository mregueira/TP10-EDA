#pragma once
struct cursorPosition
{
	int row;
	int column;
};

class basic_lcd
{
public:
	basic_lcd() {}

	virtual bool lcdInitOk() = 0;
	//virtual FT_STATUS lcdGetError() = 0; 
	virtual bool lcdClear() = 0;
	virtual bool lcdClearToEOL() = 0;
	virtual basic_lcd& operator<<(const unsigned char c) = 0;
	virtual basic_lcd& operator<<(const char * c) = 0;
	virtual bool lcdMoveCursorUp() = 0;
	virtual bool lcdMoveCursorDown() = 0;
	virtual bool lcdMoveCursorRight() = 0;
	virtual bool lcdMoveCursorLeft() = 0;
	virtual bool lcdSetCursorPosition(const cursorPosition pos) = 0;
	virtual cursorPosition lcdGetCursorPosition() = 0;
	virtual void lcdUpdateCursor() = 0; 
protected:
	int cadd;
	
	~basic_lcd(){}
};