#include <cstdio>
#include <winsock2.h>

#define SOCKET_BUFFER_SIZE sizeof(wchar_t) //max UDP datagram size

bool init()
{
	const WORD winsock_version = 0x202;
	WSADATA winsock_data;
	if (WSAStartup(winsock_version, &winsock_data))
	{
		printf("[net] WSAStartup failed: %d\n", WSAGetLastError());
		return false;
	}

	return true;
}


void main()
{
	init();

	const int address_family = AF_INET;
	const int type = SOCK_DGRAM;
	const int protocol = IPPROTO_UDP;
	SOCKET sock = socket(address_family, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		printf("socket failed: %d\n", WSAGetLastError());
		return;
	}
	printf("socket is: %i\n", sock);


	SOCKADDR_IN local_address;
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9999);
	local_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return;
	}
	printf("bind worked!");

	char buffer[SOCKET_BUFFER_SIZE];
	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d\n", WSAGetLastError());
	}
	else
	{
		buffer[bytes_received] = 0;
		printf("%d.%d.%d.%d:%d - %s",
			from.sin_addr.S_un.S_un_b.s_b1,
			from.sin_addr.S_un.S_un_b.s_b2,
			from.sin_addr.S_un.S_un_b.s_b3,
			from.sin_addr.S_un.S_un_b.s_b4,
			from.sin_port,
			buffer);
	}
}