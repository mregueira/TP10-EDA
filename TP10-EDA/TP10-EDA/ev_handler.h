#pragma once
#include <allegro5\allegro5.h>
#include <iostream>
using namespace std;

class ev_handler
{
public:
		
	ev_handler(float FPS_);
	void process_evs();
	bool is_exit();
	bool get_next_allegro_ev();
	bool start_and_reg(void);
	~ev_handler();

private:
	float FPS;

	ALLEGRO_EVENT evs;
	ALLEGRO_EVENT_QUEUE * event_queue;
	ALLEGRO_TIMER * timer;
	bool exit;
	string key_pressed;
	bool toggle_key;
	bool flush_var;
};