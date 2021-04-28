#include "tcpserver.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

int main()
{
	system("chcp 65001>nul");

	TCPServer server(20002);

	server.start();

	return 0;
}