#include "ev_handler.h"

const int width = 240;
const int height = 120;

enum {DATE_TXT, TITULAR_TXT, FINISH_TXT};
// Teclas: R, S, A, Q, +, -

ev_handler::ev_handler(float FPS_)
{
	toggle_key = 0;
	flush_var = 0;
	key_pressed = 0;

	FPS = FPS_;

	exit = false;
	show_flag = DATE_TXT;
	title_cant = 0;

	event_queue = nullptr;
	disp = nullptr;
	timer = nullptr;

}

void ev_handler::process_evs(hitachi_lcd& lcd, vector<titular>& rx_titulars)
{
	switch (evs.type) {
	case ALLEGRO_EVENT_TIMER:
		process_title(lcd, rx_titulars);
		break;
	case ALLEGRO_EVENT_KEY_UP:
		
		if (toggle_key == 1) {
			toggle_key = 0;
			key_pressed = 0;
		}
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		if (toggle_key == 0) {
			key_pressed = evs.keyboard.keycode;
			toggle_key = 1;
			flush_var = 1;
		}
		break;
	}	
	if (flush_var == 1) {

		if (key_pressed == ALLEGRO_KEY_R)
		{
			cout << "apreto R" << endl;
		}
		else if (key_pressed == ALLEGRO_KEY_S)
		{
			cout << "apreto S" << endl;
		}
		else if (key_pressed == ALLEGRO_KEY_A)
		{
			cout << "apreto A" << endl;
		}
		else if (key_pressed == ALLEGRO_KEY_Q)
		{
			cout << "apreto Q" << endl;
		}
		else if ((key_pressed == ALLEGRO_KEY_PAD_MINUS) || (key_pressed == ALLEGRO_KEY_MINUS))
		{
			cout << "apreto -" << endl;
		}
		else if (key_pressed == ALLEGRO_KEY_PAD_PLUS)
		{
			cout << "apreto +" << endl;
		}

		flush_var = 0;
	}
}

bool ev_handler::is_exit()
{
	return exit;
}

bool ev_handler::get_next_allegro_ev()
{
	return al_get_next_event(event_queue, &evs);
}

bool ev_handler::start_and_reg(void)
{
	if (!al_init()) {
		exit = true;
	}

	if (!al_install_keyboard())
	{
		exit = true;
	}

	disp = al_create_display(width, height);
	if (!disp)
	{
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		al_destroy_display(disp);
		return 1;
	}
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_destroy_timer(timer);
		al_destroy_display(disp);
		return 1;
	}

	al_start_timer(timer);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	return false;
}

void ev_handler::set_title_cant(unsigned int num)
{
	title_cant = num;
	show_flag = false;
	show_count = 0;
}


ev_handler::~ev_handler()
{
	al_destroy_timer(timer);
}

void ev_handler::process_title(hitachi_lcd & lcd, vector<titular>& rx_titulars)
{
	if ((title_cant > 0) && (!exit))
	{
		switch (show_flag)
		{
		case DATE_TXT:
			temp_tit.clear();
			lcd.lcdPrintDate(rx_titulars[show_count].pubdate);

			for (int i = 0; i < rx_titulars[show_count].fuente.size(); i++)
			{
				temp_tit += rx_titulars[show_count].fuente[i];
			}
			for (int i = 0; i < rx_titulars[show_count].titulo.size(); i++)
			{
				temp_tit += rx_titulars[show_count].titulo[i];
			}
			show_flag = TITULAR_TXT;
			break;
		case TITULAR_TXT:
			if (lcd.lcdScrollMsg(temp_tit))
			{
				show_flag = false;
				show_count++;
				if (show_count == title_cant)
				{
					exit = true;
				}
			}
			break;
		case FINISH_TXT:
			break;
		}
	}
}
