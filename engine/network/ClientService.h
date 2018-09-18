#ifndef _CE_CLIENT_SERVICE_
#define _CE_CLIENT_SERVICE_

#include <winsock2.h>

namespace CE
{
	class ClientService
	{
	public:
		static SOCKET InitClient();
		static void MakeRequest(const char *msg, SOCKET &sock);
	};
}

#endif // _CE_CLIENT_SERVICE_