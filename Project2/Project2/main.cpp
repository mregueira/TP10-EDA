#include <iostream>
#include <string>
#include "channel.h"
#include "client.h"

#include <fstream>
using namespace std;

void cortar(string &data,string &ans) {
	int pnt = 0;
	while (data.substr(pnt, 8) != "00006000") pnt++;
	pnt += 8;
	while (data.substr(pnt, 8) != "00000000") {
		ans += data[pnt];
		pnt++;
	}
}

int main() {
	client my_client;
	my_client.startConnection("news.mit.edu");

	//my_client.send_message()
	cout << '\x65' << '\n';

	//getchar();
	string msg = "GET /rss/feed HTTP/1.1\nHost: news.mit.edu\n\n";
	cout << "sending \n";
	cout << msg << '\n';
	my_client.send_message(msg.c_str(),msg.size());
	cout << "msg sent \n";
	int sz;

	string data_ans;
	my_client.receiveMessage(data_ans, 10000);

	string rss_str;
	cortar(data_ans, rss_str);

	ofstream file_test("output.txt");
	string ans_data = "";
	bool start = 0;
	for (int i = 0; i < rss_str.size(); i++) {
		if ((rss_str[i] != '\n' && rss_str[i] != '\r')  || start) {
			ans_data += rss_str[i];
			start = 1;
		}
	}
	file_test << ans_data << '\n';
	file_test.close();

	getchar();

	ifstream file("output.txt");
	string aux;
	rss_str = "";
	
	while (getline(file, aux)) {
		rss_str += aux + "\n";
	}

	/*cout << rss_str << endl;
	cout << "-----------\n";
	cout << "end of content \n";*/
	
	channel main_ch("link al que se le va a hacer get");

	main_ch.rss_test = (char*)rss_str.c_str();
	main_ch.size_of_str = rss_str.size();
	cout << rss_str.size() << endl;
	main_ch.fetch_titles();

	cout << main_ch.titles.size() << endl;


	// con estos dos super fors imprimo todos los titulos
	for (int j = 0; j < main_ch.titles.size(); j++) {
	
		for (int i = 0; i < main_ch.titles[j].titulo.size(); i++) {
			cout << main_ch.titles[j].titulo[i];
		}
		cout << endl;
		for (int i = 0; i < main_ch.titles[j].pubdate.size(); i++) {
			cout << main_ch.titles[j].pubdate[i];
		}
		cout << endl;
		for (int i = 0; i < main_ch.titles[j].fuente.size(); i++) {
			cout << main_ch.titles[j].fuente[i];
		}
		cout << endl;
	}
	

	getchar();

	//file.close();
	return 0;
}
