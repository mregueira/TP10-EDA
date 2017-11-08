#include "channel.h"

void get_str_of_month_num(string month,vector<char> &modif) {
	modif.clear();
	if (!strcmp(&month[0], "Jan")) {
		modif.push_back('0');
		modif.push_back('1');
	}
	if (!strcmp(&month[0], "Feb")) {
		modif.push_back('0');
		modif.push_back('2');
	}
	if (!strcmp(&month[0], "Mar")) {
		modif.push_back('0');
		modif.push_back('3');
	}
	if (!strcmp(&month[0], "Apr")) {
		modif.push_back('0');
		modif.push_back('4');
	}
	if (!strcmp(&month[0], "May")) {
		modif.push_back('0');
		modif.push_back('5');
	}
	if (!strcmp(&month[0], "Jun")) {
		modif.push_back('0');
		modif.push_back('6');
	}
	if (!strcmp(&month[0], "Jul")) {
		modif.push_back('0');
		modif.push_back('7');
	}
	if (!strcmp(&month[0], "Aug")) {
		modif.push_back('0');
		modif.push_back('8');
	}
	if (!strcmp(&month[0], "Sep")) {
		modif.push_back('0');
		modif.push_back('9');
	}
	if (!strcmp(&month[0], "Oct")) {
		modif.push_back('1');
		modif.push_back('0');
	}
	if (!strcmp(&month[0], "Nov")) {
		modif.push_back('1');
		modif.push_back('1');
	}
	if (!strcmp(&month[0], "Dec")) {
		modif.push_back('1');
		modif.push_back('2');
	}
}

void
start(void *userData, const char *tag, const char **vars) {
	channel * ch = (channel*)userData;
	//cout << "ENTRE A START EN ESTADO:" << ch->get_state() << endl;
	if (!strcmp("channel", tag)) {
		ch->set_state(CHANNEL);
	}
	
	if (!strcmp("item", tag)) {
		ch->set_state(ITEM);
	}
	
	if (!strcmp("title",tag)) {
		if (ch->get_state() == CHANNEL) {
			ch->set_state(CH_TITLE);
		}
		else if (ch->get_state() == ITEM) {
			ch->set_state(I_TITLE);
		}
	}

	if (!strcmp("pubDate", tag)) {
	//	cout << " hay un pub date!!! " << endl;
		if (ch->get_state() == ITEM) {
			ch->set_state(I_PUBDATE);
		}
	}
	//cout << "SALI DE START EN ESTADO:" << ch->get_state() << endl;
}

void
end(void *userData, const XML_Char *el)
{
	
	channel *ch = (channel*)userData;

	//cout << "ENTRE A END EN ESTADO:" << ch->get_state() << endl;

	
	if (!strcmp("channel", el)) {
		ch->set_state(DEFAULT_ST_VALUE); // O sea cierro un channel y ?¡?¡
	}
	
	if (!strcmp("item", el)) {
		ch->set_state(CHANNEL);  // TENGO DUDA CON ESTO si cierro un item significa que estoy en un channel?
	}
	
	if (!strcmp("title", el)) {
		
		if (ch->get_state() == CH_TITLE) {
			ch->set_state(CHANNEL);
		}
		else if (ch->get_state() == I_TITLE) {
			ch->set_state(ITEM);
		}
	}
	if (!strcmp("pubDate", el)) {
		if (ch->get_state() == I_PUBDATE) {
		//	cout << "entre a pubdate ESTANDO EN END" << endl;
			if ( ch->temptit.fuente.size() > 0 && ch->temptit.titulo.size() > 0) {
				// dsp voy a tener que fijarme si pubdate no es >0
				ch->titles.push_back(ch->temptit);
				ch->temptit.pubdate.clear();
				ch->temptit.titulo.clear();
			}
			ch->set_state(ITEM);
		}
	}
	//cout << "SALI DE END EN ESTADO:" << ch->get_state() << endl;

}
void
handle_data(void *data, const char *content, int length)
{
	channel *ch = (channel*)data;
	if (ch->get_state() == CH_TITLE) { // ESTA ES LA FUENTE
		vector<char> fuente_actual;
		for (int i = 0; i < length; i++) {
			fuente_actual.push_back(content[i]);
			//ch->get_next_titular();
			// con esto deberia obtener los titulares
		}
		ch->temptit.fuente = fuente_actual;
	}
	if (ch->get_state() == I_TITLE) {   // ESTOS SON LOS TITULARES EFECTIVAMENTE
		vector<char> actual_title;
		for (int i = 0; i < length; i++) {
			actual_title.push_back(content[i]);
			//ch->get_next_titular();
			// con esto deberia obtener los titulares
		}
		ch->temptit.titulo = actual_title;
	}

	if (ch->get_state() == I_PUBDATE) {   // Obtengo el pubdate
		vector<char> aux_day;
		string aux_str_month;
		vector<char>aux_month;
		vector<char> aux_year;
		vector<char> aux_hour;
		vector<char> aux_min;

		aux_day.push_back(content[5]); // pusheo el day
		aux_day.push_back(content[6]);
		
		aux_str_month.push_back(content[8]);
		aux_str_month.push_back(content[9]);
		aux_str_month.push_back(content[10]);

		get_str_of_month_num(aux_str_month,aux_month);

		aux_year.push_back(content[14]); // pusheo el año
		aux_year.push_back(content[15]);	

		aux_hour.push_back(content[17]); // pusheo hora y minuto
		aux_hour.push_back(content[18]);
		aux_min.push_back(content[20]);
		aux_min.push_back(content[21]);
		

		vector<char> actual_pubdate;
		for (int i = 0; i < aux_day.size(); i++) {
			actual_pubdate.push_back(aux_day[i]);
		}
		actual_pubdate.push_back('/');
		for (int i = 0; i < aux_month.size(); i++) {
			actual_pubdate.push_back(aux_month[i]);
		}
		actual_pubdate.push_back('/');
		for (int i = 0; i < aux_year.size(); i++) {
			actual_pubdate.push_back(aux_year[i]);
		}
		actual_pubdate.push_back('-');
		for (int i = 0; i < aux_hour.size(); i++) {
			actual_pubdate.push_back(aux_hour[i]);
		}
		actual_pubdate.push_back(':');
		for (int i = 0; i < aux_min.size(); i++) {
			actual_pubdate.push_back(aux_min[i]);
		}
		ch->temptit.pubdate = actual_pubdate;
	}
	// si no esta vacio NINGUNO entonces -> OK!! pusheo el titular al vector de titulares

	// Warning  : asumo que todos los titulares tienen pubdate y que la pubdate siempre va a lo ultimo
}


channel::channel(const char* str)
{
	link = str;
	set_state(DEFAULT_ST_VALUE);
}

void channel::fetch_titles()
{
	XML_Parser p = XML_ParserCreate(NULL);
	XML_SetElementHandler(p, start, end);  // llamamos a los callbacks correspondientes 
	XML_SetCharacterDataHandler(p, handle_data);

	XML_SetUserData(p, (void*) this);

/*Para qué el Userdata : Communicating between handlers*/


	/*	rssr(link); // supongo que es rss read
	if (r.success()) {
	XML_Status s;
	XML_Parse(p, r.getRss(), rss.size, true);

	}
	*/
	//ASUMIENDO QUE YA TENGO EL RSS 

	XML_Parse(p, rss_test, size_of_str, true);


}

state channel::get_state(void)
{
	return st;
}

void channel::set_state(state n)
{
	st = n;
}

void channel::print_char_vec(vector<char> gg)
{
	for (int i = 0; i < gg.size(); i++) {
		cout << gg[i];
	}
}


channel::~channel()
{
}
