#include <iostream>
#include <string>
#include "channel.h"
#include <fstream>
using namespace std;

int main() {
	ifstream file("hola4.txt");
	
	string rss_str , aux;
	while (getline(file, aux)) {
		rss_str += aux + "\n";
	}

//	cout << rss_str << endl;
	
	
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

	file.close();
	return 0;
}
