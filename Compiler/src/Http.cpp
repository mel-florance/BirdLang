#include "pch.h"
#include "Http.h"
#include "Url.h"

#ifdef PLATFORM_WINDOWS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>
#pragma comment(lib,"ws2_32.lib")
#endif

std::string Http::get(const Url& url)
{
	WSADATA data;
	SOCKET Socket;
	SOCKADDR_IN addr;
	struct hostent* host;

	char buffer[10000];
	std::string headers;
	std::string out;

	auto path = std::string(url.Path.begin(), url.Path.end());


	if (path.size() == 0)
		path = '/';

	headers = "GET " + path + " HTTP/1.1\r\nHost: " +
		std::string(url.Host.begin(), url.Host.end()) +
		"\r\nConnection: close\r\n\r\n";

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		std::cout << "WSAStartup failed.\n";
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(std::string(url.Host.begin(), url.Host.end()).c_str());

	addr.sin_port = htons(80);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	if (connect(Socket, (SOCKADDR*)(&addr), sizeof(addr)) != 0) {
		std::cout << "Could not connect";
	}

	send(Socket, headers.c_str(), strlen(headers.c_str()), 0);

	int length;
	while ((length = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;

		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			out += buffer[i];
			i += 1;
		}
	}

	closesocket(Socket);
	WSACleanup();

	return out;
}
