#include "hitachi_lcd.h"
#include "ev_handler.h"
#include "channel.h"

#define FPS_DEF 50.0

int main()
{
	hitachi_lcd lcd;
	ev_handler ev(FPS_DEF);
	ev.start_and_reg();
	channel ch("LINK");
	ch.titles.push_back(ch.temptit); // Provisorio

	string tit1 = "The quick brown fox";
	string src1 = "Fuente: ";
	string date1 = "08/11/17 - 12:34";
	//////////////////
	for (int i = 0; i < src1.size(); i++)
	{
		ch.titles[0].fuente.push_back(src1[i]);
	}
	for (int j = 0; j < tit1.size(); j++) 
	{
		ch.titles[0].titulo.push_back(tit1[j]);
	}
	for (int k = 0; k < date1.size(); k++)
	{
		ch.titles[0].pubdate.push_back(date1[k]);
	}
	//////////////////
	if (lcd.lcdInitOk())
	{
		lcd << "En EDA no anda nada";
	}
	cursorPosition aux;
	
	aux.row = 1;
	aux.column = 0;
	lcd.lcdSetCursorPosition(aux);
	ev.set_title_cant(1);

	while (!ev.is_exit())
	{
		if (ev.get_next_allegro_ev())
		{
			ev.process_evs(lcd, ch.titles);
		}
	}

	return 0;
}