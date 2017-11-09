#define _WIN32_WINNT 0x0501

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <boost/exception_ptr.hpp> 

using namespace std;

class client {
	public:
		client();
		void startConnection(const char* host);
		void receiveMessage(string &ans, int maxsize);
		void send_message(const char *msg, int size);
		~client();
		bool success();

	private:
		boost::asio::io_service* IO_handler;
		boost::asio::ip::tcp::socket* socket_forClient;
		boost::asio::ip::tcp::resolver* client_resolver;
		boost::asio::ip::tcp::resolver::iterator endpoint;
		
		bool failure;

};

#endif
