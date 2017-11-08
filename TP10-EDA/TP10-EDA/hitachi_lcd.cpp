#include "hitachi_lcd.h"
#include <chrono>
#define HITACHI_LINE 40
#define LCD_LINE 16

#define LCD_DESCRIPTION "EDA LCD 1 B"
#define CONNECTING_TIME 5 //in seconds

hitachi_lcd::hitachi_lcd()
{
	bool found = false;
	lcd_stat = !FT_OK;
	cadd = 1;
	error_log = true; // No usable LCD at start
	std::chrono::seconds MaxTime(CONNECTING_TIME);/*The display has a settling time after the physical connection so the attempt to connect
												  will be done for a few seconds*/

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> current = start;

	while (lcd_stat != FT_OK && ((current - start) < MaxTime))//loop till succesful connection o max connecting time is exceeded
	{
		lcd_stat = FT_OpenEx((void *)LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, &Handle);

		if (lcd_stat == FT_OK)
		{
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(Handle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
			{
				error_log = false;
				init_4_bit_mode(Handle);
				lcdClear();
			}
			else
				printf("Couldn't configure LCD\n");
		}
		current = std::chrono::system_clock::now();
	}

	if (lcd_stat != FT_OK)
		printf("Couldn't open LCD\n");
}

FT_STATUS hitachi_lcd::write_nybble_to_ir(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	DWORD bytes_sent;
	UCHAR temp;
	temp = (data << 4) & 0xF0;
	temp = (temp | RS_IR) & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret= FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp& (~ENABLE);
	FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1);
	return ret;
}

FT_STATUS hitachi_lcd::write_nybble_to_dr(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	DWORD bytes_sent;
	UCHAR temp;
	temp = (data << 4) & 0xF0;
	temp = (temp | RS_DR) & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp & (~ENABLE);
	FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1);
	return ret;
}

FT_STATUS hitachi_lcd::write_byte_to_ir(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	UCHAR temp;
	temp = (data >> 4) & 0x0F;
	ret = write_nybble_to_ir(h, temp);
	temp = data & 0x0F;
	ret = write_nybble_to_ir(h, temp);

	return ret;
}

FT_STATUS hitachi_lcd::write_byte_to_dr(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	UCHAR temp;
	temp = (data >> 4) & 0x0F;
	ret = write_nybble_to_dr(h, temp);
	temp = data & 0x0F;
	ret = write_nybble_to_dr(h, temp);

	return ret;
}

void hitachi_lcd::clr_display(FT_HANDLE h)
{
	lcd_stat = write_byte_to_ir(h, LCD_CLEAR);
}


void hitachi_lcd::init_4_bit_mode(FT_HANDLE h)
{
	lcd_stat = write_nybble_to_ir(h, 0x02);

	lcd_stat = write_nybble_to_ir(h, 0x02);
	lcd_stat = write_nybble_to_ir(h, 0x08);

	lcd_stat = write_nybble_to_ir(h, 0x00);
	lcd_stat = write_nybble_to_ir(h, 0x0E);

	lcd_stat = write_nybble_to_ir(h, 0x00);
	lcd_stat = write_nybble_to_ir(h, 0x06);
}

bool hitachi_lcd::lcdInitOk()
{
	return !error_log;
}

bool hitachi_lcd::lcdClear()
{
	clr_display(Handle);
	cadd = 1;
	return true;
}

bool hitachi_lcd::lcdClearToEOL()
{
	if (cadd%LCD_LINE == 0)
	{
		return false;
	}
	else
	{
		int aux = 0;
		if (cadd <= LCD_LINE)
		{
			for (aux = 0; aux < (LCD_LINE - cadd); aux++)
			{
				lcd_stat = write_byte_to_dr(Handle, EMPTY_CHAR);
			}
			for (int i = 0; i < aux; i++)
			{
				lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
			}
		}
		else
		{
			for (aux = 0; aux < ((2*LCD_LINE) - cadd); aux++)
			{
				lcd_stat = write_byte_to_dr(Handle, EMPTY_CHAR);
			}
			for (int i = 0; i < aux; i++)
			{
				lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
			}
		}
		return true;
	}
}

basic_lcd & hitachi_lcd::operator<<(const unsigned char c)
{
	this->write_byte_to_dr(Handle, int(c));
	if (cadd%LCD_LINE == 0)
	{
		for (int i = 0; i < (HITACHI_LINE - LCD_LINE); i++)
		{
			lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		}
		if (cadd == (2 * LCD_LINE))
		{
			cadd = 1;
		}
		else
		{
			cadd++;
		}
	}
	else
	{
		cadd++;
	}
	return *this;
}

basic_lcd & hitachi_lcd::operator<<(const char * c)
{
	unsigned int i = 0;
	while (c[i] != NULL)
	{
		this->write_byte_to_dr(Handle, int(c[i]));
		i++;
		if (cadd%LCD_LINE == 0)
		{
			for (int i = 0; i < (HITACHI_LINE - LCD_LINE); i++)
			{
				lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
			}
			if (cadd == (2 * LCD_LINE))
			{
				cadd = 1;
			}
			else
			{
				cadd++;
			}
		}
		else
		{
			cadd++;
		}
	}
	return *this;
}

bool hitachi_lcd::lcdMoveCursorUp()
{
	if (cadd <= LCD_LINE)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < HITACHI_LINE; i++)
		{
			lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
		}
		cadd -= LCD_LINE;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorDown()
{
	if (cadd >= (LCD_LINE + 1))
	{
		return false;
	}
	else
	{
		for (int i = 0; i < HITACHI_LINE; i++)
		{
			lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		}
		cadd += LCD_LINE;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorRight()
{
	if (cadd%LCD_LINE == 0)
	{
		return false;
	}
	else
	{
		lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		cadd++;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorLeft()
{
	if ((cadd == 1) || (cadd == (LCD_LINE + 1)))
	{
		return false;
	}
	else
	{
		lcd_stat = write_byte_to_ir(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
		cadd--;
		return true;
	}
}

bool hitachi_lcd::lcdSetCursorPosition(const cursorPosition pos)
{
	if ((pos.row < 0) || (pos.row > 1) || (pos.column < 0) || (pos.column > 15))
	{
		return false;
	}
	else
	{
		if (cadd < (LCD_LINE+1))
		{
			if (pos.row == 1)
			{
				lcdMoveCursorDown();
				while (cadd != (LCD_LINE + 1))
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1 + LCD_LINE))
				{
					lcdMoveCursorRight();
				}
			}
			else
			{
				while (cadd != 1)
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1))
				{
					lcdMoveCursorRight();
				}
			}
		}
		else
		{
			if (pos.row == 0)
			{
				lcdMoveCursorUp();
				while (cadd != 1)
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1))
				{
					lcdMoveCursorRight();
				}
			}
			else
			{
				while (cadd != (LCD_LINE + 1))
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1 + LCD_LINE))
				{
					lcdMoveCursorRight();
				}
			}
		}
		return true;
	}
}

cursorPosition hitachi_lcd::lcdGetCursorPosition()
{
	cursorPosition aux;
	if (cadd > LCD_LINE)
	{
		aux.row = 1;
		aux.column = cadd - LCD_LINE - 1;
	}
	else
	{
		aux.row = 0;
		aux.column = cadd - 1;
	}
	return aux;
}

hitachi_lcd::~hitachi_lcd()
{
	FT_Close(Handle);
}