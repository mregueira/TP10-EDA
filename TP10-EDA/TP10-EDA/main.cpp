#include "client.h"
#include "hitachi_lcd.h"
#include "ev_handler.h"
#include "channel.h"

#include <fstream>
#include <string>

#define FPS_DEF 50.0

using namespace std;

void cortar(string &data, string &ans) {
	int pnt = 0;
	while (data.substr(pnt, 8) != "00006000") pnt++;
	pnt += 8;
	while (data.substr(pnt, 8) != "00000000") {
		ans += data[pnt];
		pnt++;
	}
}

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		cout << ">> Input error: to many arguments." << endl;
		cout << ">> Only needed the host for parse.-" << endl;
		return -1;
	}

	if (argc < 2)
	{
		cout << ">> Input error: no host entered" << endl;
		cout << ">> Please enter host as parameter.-" << endl;
		return -1;
	}

	client my_client;
	my_client.startConnection(argv[1]);
	
	if (!my_client.success())
	{
		return -1;
	}

	string msg = "GET /rss/feed HTTP/1.1\nHost: news.mit.edu\n\n";
	cout << ">> Sending request... \n";
	cout << msg << '\n';
	my_client.send_message(msg.c_str(), msg.size());
	cout << ">> Message sent \n";

	string data_ans;
	my_client.receiveMessage(data_ans, 10000);

	string rss_str;
	cortar(data_ans, rss_str);

	ofstream file_test("output.txt");
	string ans_data = "";
	bool start = 0;
	for (int i = 0; i < rss_str.size(); i++) {
		if ((rss_str[i] != '\n' && rss_str[i] != '\r') || start) {
			ans_data += rss_str[i];
			start = 1;
		}
	}
	file_test << ans_data << '\n';
	file_test.close();

	ifstream file("output.txt");
	string aux;
	rss_str = "";

	while (getline(file, aux)) {
		rss_str += aux + "\n";
	}

	channel main_ch(argv[1]);

	main_ch.rss_test = (char*)rss_str.c_str();
	main_ch.size_of_str = rss_str.size();
	main_ch.fetch_titles();

	cout << ">> " << main_ch.titles.size() << " titles found" << endl;

	hitachi_lcd lcd;
	ev_handler ev(FPS_DEF);
	ev.start_and_reg();
	

	if (!lcd.lcdInitOk())
	{
		cout << ">> LCD init error" << endl;
		cout << ">> Check LCD connection.-" << endl;
		return -1;
	}
	cursorPosition cursi;
	
	cursi.row = 1;
	cursi.column = 0;
	lcd.lcdSetCursorPosition(cursi);
	ev.set_title_cant(main_ch.titles.size());

	while (!ev.is_exit())
	{
		ev.get_next_allegro_ev();
		ev.process_evs(lcd, main_ch.titles);
	}
	return 0;
}