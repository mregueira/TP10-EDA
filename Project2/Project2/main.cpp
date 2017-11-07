#include <iostream>
#include <string>
using namespace std;

#include "channel.h"

int main() {

	channel main_ch("link al que se le va a hacer get");

	main_ch.rss_test = "aca deberia ir el contenido de un xml";
	main_ch.fetch_titles();

	getchar();
	return 0;
}
