#include <iostream>
#include <string>
#include "channel.h"
#include <fstream>
using namespace std;

int main() {
	ifstream file("hola.txt");
	
	string rss_str , aux;
	while (getline(file, aux)) {
		rss_str += aux + "\n";
	}

	cout << rss_str << endl;
	
	
	channel main_ch("link al que se le va a hacer get");

	main_ch.rss_test = (char*)rss_str.c_str();
	main_ch.size_of_str = rss_str.size();
	cout << rss_str.size() << endl;
	main_ch.fetch_titles();

	for (int i = 0; i < main_ch.titles.size(); i++) {
		for (int j = 0; j < main_ch.titles[i].fuente.size(); j++) {
			cout << main_ch.titles[i].fuente[j];
		}
		cout << endl;
	}

	getchar();

	file.close();
	return 0;
}
