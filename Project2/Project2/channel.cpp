#include "channel.h"

vector<char> get_str_of_month_num(vector<char> month_real) {
	string month;
	for (int i = 0; i < month_real.size(); i++) {
		month.push_back(month_real[i]);
	}
	string aux;
	if (!strcmp(&month[0], "Jan")) {
		aux = "01";
	}
	if (!strcmp(&month[0], "Feb")) {
		aux = "02";
	}
	if (!strcmp(&month[0], "Mar")) {
		aux = "03";
	}
	if (!strcmp(&month[0], "Apr")) {
		aux = "04";
	}
	if (!strcmp(&month[0], "May")) {
		aux = "05";
	}
	if (!strcmp(&month[0], "Jun")) {
		aux = "06";
	}
	if (!strcmp(&month[0], "Jul")) {
		aux = "07";
	}
	if (!strcmp(&month[0], "Aug")) {
		aux = "08";
	}
	if (!strcmp(&month[0], "Sep")) {
		aux = "09";
	}
	if (!strcmp(&month[0], "Oct")) {
		aux = "10";
	}
	if (!strcmp(&month[0], "Nov")) {
		aux = "11";
	}
	if (!strcmp(&month[0], "Dec")) {
		aux = "12";
	}

	vector<char> ret;
	for (int i = 0; i < month_real.size(); i++) {
		ret.push_back(aux[i]);
	}
	return ret;
}

void
start(void *userData, const char *tag, const char **vars) {
	channel * ch = (channel*)userData;
	cout <<" < " <<  tag << endl;
	if (!strcmp("channel", tag)) {
		ch->set_state(CHANNEL);
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
		if (ch->get_state() == ITEM) {
			ch->set_state(I_PUBDATE);
		}
	}

}

void
end(void *userData, const XML_Char *el)
{
	channel *ch = (channel*)userData;
	cout << el << "  || > " << endl;
	if (!strcmp("channel", el)) {
		ch->set_state(CHANNEL);
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
			if (ch->temptit.fuente.size() > 0 && ch->temptit.pubdate.size() > 0 && ch->temptit.titulo.size() > 0) {
				ch->titles.push_back(ch->temptit);
				ch->temptit.fuente.clear();
				ch->temptit.pubdate.clear();
				ch->temptit.titulo.clear();
			}
			ch->set_state(ITEM);
		}
	}

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
		vector<char> aux_month;
		vector<char> aux_year;
		vector<char> aux_hour;
		vector<char> aux_min;

		aux_day.push_back(content[5]); // pusheo el day
		aux_day.push_back(content[6]);
		
		aux_month.push_back(content[9]);
		aux_month.push_back(content[10]);
		aux_month.push_back(content[11]);

		aux_day = get_str_of_month_num(aux_day);

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


channel::~channel()
{
}
