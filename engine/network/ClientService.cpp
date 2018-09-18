#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ClientService.h"

#include <cstdio>

#include "..\engine\common\NetworkConfig.h"



namespace CE
{
	SOCKET ClientService::InitClient()
	{
		const int address_family = AF_INET;
		const int type = SOCK_DGRAM;
		const int protocol = IPPROTO_UDP;

		SOCKET sock = socket(address_family, type, protocol);

		SOCKADDR_IN src_addr;
		src_addr.sin_family = AF_INET;
		src_addr.sin_port = htons(1234);
		src_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		const int srcAddressSize = sizeof(src_addr);


		if (bind(sock, (SOCKADDR*)&src_addr, srcAddressSize) == SOCKET_ERROR)
		{
			printf("client bind failed: %d\n", WSAGetLastError());
			return NULL;
		}
		printf("client bind worked!\n");
		return sock;
	}

	void ClientService::MakeRequest(const char *msg, SOCKET &sock)
	{

		SOCKADDR_IN server_address;
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(NetworkConfig::serverPort);
		server_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		const int serverAddressSize = sizeof(server_address);

		char message[NetworkConfig::socketBufferSize];
		strcpy_s(message, NetworkConfig::socketBufferSize, msg);

		int flags = 0;
		if (sendto(sock, message, sizeof(message), flags, reinterpret_cast<SOCKADDR*>(&server_address), serverAddressSize) == SOCKET_ERROR)
		{
			printf("sendto failed: %d", WSAGetLastError());
		}
	}

}
