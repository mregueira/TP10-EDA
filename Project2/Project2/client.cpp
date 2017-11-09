
#include "client.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <boost/exception_ptr.hpp> 
#include <chrono>

const string CLIENT_PORT = "80";


client::client() {
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
	failure = 0;
}

client::~client() {
	//cout << "closing client\n";
	socket_forClient->close();
	delete client_resolver;
	delete socket_forClient;
	delete IO_handler;
}

void client::startConnection(const char* host) {
	try {
		cout << "Trying to connect to " << host << " on port " << CLIENT_PORT << std::endl;
		endpoint = client_resolver->resolve(
			boost::asio::ip::tcp::resolver::query(host, CLIENT_PORT));
		boost::asio::connect(*socket_forClient, endpoint);
		//socket_forClient->non_blocking(true);
	}catch (std::exception const&  ex){
		cout << "could not connect"<< '\n';
		failure = 1;
	}
}

bool client::success() {
	return !failure;
}

void client::receiveMessage(string &ans,int maxsize) {
	boost::system::error_code error;

	while (ans.find("00000000") == string::npos) {

		char buf[10000];
		size_t len = socket_forClient->read_some(boost::asio::buffer(buf, 10000), error);

		int size_buf = strlen(buf);
		for (int i = 0; i < size_buf; i++) {
			//cout << buf[i] << '\n';
			ans += buf[i];
		}
	}
	//buf[0] = '\0';
			
	cout << ans.size() << '\n';
	if (!error) {
		//std::cout << std::endl << "Server said: " << buf << std::endl;
	} else {
		//std::cout << "Error while trying to connect to server " << error.message() << std::endl;
		failure = 1;
		return;
	}

}

void client::send_message(const char *msg, int size) {
	
	//cout << size << '\n';
	size_t len;
	boost::system::error_code error;
	cout << "sending " << msg << '\n';
	len = socket_forClient->write_some(boost::asio::buffer(msg, size), error);

}