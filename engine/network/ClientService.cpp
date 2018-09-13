#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ClientService.h"

#include <cstdio>
#include <winsock2.h>

#include "..\engine\common\NetworkConfig.h"



namespace CE
{
	void ClientService::MakeRequest(const char *msg)
	{
		const int address_family = AF_INET;
		const int type = SOCK_DGRAM;
		const int protocol = IPPROTO_UDP;
		SOCKET sock = socket(address_family, type, protocol);

		SOCKADDR_IN server_address;
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(NetworkConfig::serverPort);
		server_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		int serverAddressSize = sizeof(server_address);


		char message[NetworkConfig::socketBufferSize];
		strcpy_s(message, NetworkConfig::socketBufferSize, msg);

		int flags = 0;
		if (sendto(sock, message, sizeof(message), flags, reinterpret_cast<SOCKADDR*>(&server_address), serverAddressSize) == SOCKET_ERROR)
		{
			printf("sendto failed: %d", WSAGetLastError());
		}
	}

}
