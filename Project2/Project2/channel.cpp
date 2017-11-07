#include "channel.h"

void
start(void *userData, const char *tag, const char **vars) {
	channel * ch = (channel*)userData;
	if ("title" == tag) {
		if (ch->get_state() == CHANNEL) {
			ch->set_state(CH_TITLE);
		}
		else if (ch->get_state() == ITEM) {
			ch->set_state(I_TITLE);
		}
	}
}

void
end(void *userData, const XML_Char *el)
{
}
void
handle_data(void *data, const char *content, int length)
{

}


channel::channel(const char* st)
{
	link = st;
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

	XML_Parse(p, rss_test, 10, true);

	cout << "probando XML_PARSE" << endl;

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
