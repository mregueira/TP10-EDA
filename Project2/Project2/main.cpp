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

	//cout << data_ans << '\n';
	string rss_str;
	cortar(data_ans, rss_str);
	//cout << rss_str << '\n';
	//cout << data_ans << '\n';
	//ofstream file_test("output.txt");
	//file_test << rss_str << '\n';
	//file_test.close();

	getchar();
	//return 0;

	//return 0;
	///ofstream file_test("output.txt");
	//file_test << rss_str << '\n';
	//return 0;

	//getchar();
	//

	//cortar(data_ans);

	/*getchar();
	return 0;

	
	
	string rss_str , aux;
	strin aux;
	*/

	ifstream file("output.txt");
	string aux;
	rss_str = "";
	while (getline(file, aux)) {
		rss_str += aux;
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
