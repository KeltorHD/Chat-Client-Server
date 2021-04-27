#pragma once

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <functional>
#include <thread>
#include <mutex>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class TCPServer
{
public:
	class TCPClient;

	TCPServer(unsigned port);
	~TCPServer();

	void start();

private:
	unsigned port;
	SOCKET S; //дескриптор прослушивающего сокета
	SOCKET NS;
	sockaddr_in serv_addr;
	WSADATA wsadata;
	std::list<std::thread> threads;
	std::list<std::pair<std::shared_ptr<TCPClient>, std::string>> clients;
	std::mutex print_mutex;
	std::mutex vec_mutex;
	std::vector<size_t> del;

	void client_loop(std::shared_ptr<TCPClient> client);

	void send_msg_all(const std::string& from, const std::string& msg);
};

class TCPServer::TCPClient
{
public:
	TCPClient(SOCKET S, sockaddr_in cli_addr);
	~TCPClient();

	const std::string& get_data();
	void send_data(const std::string& text);

	friend class TCPServer;

private:
	SOCKET S; //сокет клиента
	sockaddr_in cli_addr;
	std::string data;

	const std::string& get_data(bool& err);
};