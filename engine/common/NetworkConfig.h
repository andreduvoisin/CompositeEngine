#ifndef _CE_NETWORK_CONFIG_H_
#define _CE_NETWORK_CONFIG_H_

namespace CE
{
	class NetworkConfig
	{
	public:
		static const int socketBufferSize;
		static const int serverPort;
	};

	const int NetworkConfig::socketBufferSize = sizeof(unsigned short int);
	const int NetworkConfig::serverPort = 9999;
}

#endif // _CE_NETWORK_CONFIG_H_