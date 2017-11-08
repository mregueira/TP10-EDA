#include "hitachi_lcd.h"
#include "ev_handler.h"

#define FPS_DEF 50.0

int main()
{
	hitachi_lcd lcd;
	ev_handler ev(FPS_DEF);
	ev.start_and_reg();

	if (lcd.lcdInitOk())
	{
		lcd << "En EDA no anda nada";
	}
	cursorPosition aux;
	aux.row = 1;
	aux.column = 0;
	lcd.lcdSetCursorPosition(aux);

	while (1)
	{
		if (ev.get_next_allegro_ev())
		{
			ev.process_evs(lcd, "The quick brown fox jumps over the lazy dog");
		}
	}

	return 0;
}