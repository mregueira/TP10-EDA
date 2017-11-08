#include "ev_handler.h"

// Teclas: R, S, A, Q, +, -

ev_handler::ev_handler(float FPS_)
{
	toggle_key = 0;
	flush_var = 0;
	key_pressed = "INIT";

	FPS = FPS_;

	exit = false;

	event_queue = nullptr;
	timer = nullptr;
}

void ev_handler::process_evs()
{
	switch (evs.type) {

	case ALLEGRO_EVENT_KEY_UP:
		
		if (toggle_key == 1) {
			toggle_key = 0;
			key_pressed = "RESET";
		}
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		if (toggle_key == 0) {
			key_pressed = al_keycode_to_name(evs.keyboard.keycode);
			toggle_key = 1;
			flush_var = 1;
		}
		break;
	}	
	if (flush_var == 1) {

		if (!strcmp(key_pressed.c_str(), "R"))
		{
			cout << "apreto R" << endl;
		}
		else if (!strcmp(key_pressed.c_str(), "S"))
		{
			cout << "apreto S" << endl;
		}
		else if (!strcmp(key_pressed.c_str(), "A"))
		{
			cout << "apreto A" << endl;
		}
		else if (!strcmp(key_pressed.c_str(), "Q"))
		{
			cout << "apreto Q" << endl;
		}
		else if (!strcmp(key_pressed.c_str(), "+"))
		{
			cout << "apreto +" << endl;
		}
		else if (!strcmp(key_pressed.c_str(), "-"))
		{
			cout << "apreto -" << endl;
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

	//timer = al_create_timer(1.0 / FPS);
	//if (!timer) {
	//	return 1;
	//}
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_destroy_timer(timer);
		return 1;
	}

	//al_start_timer(timer);
	//al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	return false;
}


ev_handler::~ev_handler()
{
	al_destroy_timer(timer);
}