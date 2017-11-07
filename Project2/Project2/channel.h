#pragma once
#include <iostream>
#include "expat.h"
#include <string>
#include "titular.h"
#include <vector>

enum state {TAG,CHANNEL,ITEM,CH_TITLE,I_TITLE}; // hay que rellenar esto me parece que falta

class channel
{
public:
	channel(const char* = NULL );
	void fetch_titles();
	bool noError();
	const char* get_error_str();
	bool title_s_empty();
	state get_state(void);
	void set_state(state n);
	char * rss_test;

	titular get_next_titular();
	~channel();
private:
	string chname;
	vector <titular> titles; 
	string link;
	string error;
	state st;
	titular *temptit;
};

