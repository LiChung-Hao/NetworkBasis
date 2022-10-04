#pragma once
#include "iostream"
#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

using namespace std;
class NetworkManager
{
public:
	static NetworkManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new NetworkManager();
		}
		return instance;
	}

	void Init();
	void ShutDown();
	void CreateUDPSockets();
	void BindUDP();
	void SetRemoteData();
	void SendDataUDP(const char* message);
	int ReceiveDataUDP(char* message);
	

	const int MAX_MSG_SIZE = 65355;

private:
	NetworkManager();
	~NetworkManager();

	static NetworkManager* instance;

	SOCKET UDPSocketIn;
	SOCKET UDPSocketOut;

	SOCKADDR_IN UDPOutAddr;
	SOCKADDR_IN UDPInAddr;

};

