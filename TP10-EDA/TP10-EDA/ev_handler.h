#pragma once
#include <allegro5\allegro5.h>
#include <iostream>
#include "hitachi_lcd.h"
#include "titular.h"

using namespace std;

class ev_handler
{
public:
		
	ev_handler(float FPS_);
	void process_evs(hitachi_lcd& lcd, vector<titular>& rx_titulars);
	bool is_exit();
	bool get_next_allegro_ev();
	bool start_and_reg(void);
	void set_title_cant(unsigned int num);
	~ev_handler();

private:
	double FPS;

	ALLEGRO_EVENT evs;
	ALLEGRO_EVENT_QUEUE * event_queue;
	ALLEGRO_DISPLAY *disp;
	bool exit;
	int key_pressed;
	bool toggle_key;
	bool flush_var;

	unsigned int title_cant;
	unsigned int show_flag;
	unsigned int show_count;
	unsigned int end_delay;
	string temp_tit;
	void process_title(hitachi_lcd& lcd, vector<titular>& in_titles);
};