#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include "expat.h"
#include <string>
#include "titular.h"
#include <vector>

enum state {CHANNEL=0,ITEM,CH_TITLE,I_TITLE,I_PUBDATE, DEFAULT_ST_VALUE,ITEM2}; // hay que rellenar esto me parece que falta

class channel
{
public:
	channel(const char* = NULL );
	void fetch_titles();
	state get_state(void);
	void set_state(state n);
	void print_char_vec(vector<char>);


	bool noError();
	const char* get_error_str();
	bool title_s_empty();
	titular get_next_titular();

	~channel();

	titular temptit;

	vector <titular> titles;
	// ESTO VAMOS A TENER QUE SACARLO CUANDO SE INCORPORE RSS
	int size_of_str;
	char * rss_test;
private:
	string chname; 
	string link;
	string error;
	state st;
	
};

#endif