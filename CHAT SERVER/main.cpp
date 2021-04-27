#include "tcpserver.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

std::vector<std::string> delim(std::string str, std::string delim)
{
	std::vector<std::string> arr;
	size_t prev = 0;
	size_t next;
	size_t delta = delim.length();

	while ((next = str.find(delim, prev)) != std::string::npos)
	{
		arr.push_back(str.substr(prev, next - prev));
		prev = next + delta;
	}

	arr.push_back(str.substr(prev));

	return arr;
}

bool includes(std::string inc, std::string str)
{
	for (size_t i = 0; i < min(str.length(), inc.length()); i++)
	{
		if (inc[i] != str[i])
			return false;
	}
	return true;
}

int main()
{
	TCPServer server(20002);

	server.start();

	return 0;
}