#include "NetworkManager.h"
#include "WS2tcpip.h"


NetworkManager* NetworkManager::instance = nullptr;
NetworkManager::NetworkManager()
{}
NetworkManager::~NetworkManager()
{}

void NetworkManager::Init()
{
	cout << "NetworkManager init" << endl;

	WSADATA lpWSAData;
	int error = WSAStartup(MAKEWORD(2, 2), &lpWSAData);

	if (error != 0)
	{
		cout << "WSA failed with error: " << WSAGetLastError() << endl;
	}
}

void NetworkManager::ShutDown()
{
	cout << "NetworkManager::ShutDown() called." << endl;
	int error = WSAGetLastError(); //call this func once at the very beginning of the shutdown func

	if (error != 0)
	{
		cout << "WSA failed with error: " << error << endl;
	}

	if (UDPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{
			cout << "Error! Closing UDPSocket in!!" << endl;
		}
	}

	if (UDPSocketOut != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketOut) != 0)
		{
			cout << "Error! Closing UDPSocket out!!" << endl;
		}
	}
	WSACleanup();
	exit(0);
}

void NetworkManager::BindUDP()
{
	//using IPV4
	UDPInAddr.sin_family = AF_INET;
	UDPInAddr.sin_port = htons(8889);
	UDPInAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = bind(UDPSocketIn, reinterpret_cast<SOCKADDR*>(&UDPInAddr),sizeof(UDPInAddr));
	if (bindError == SOCKET_ERROR)
	{
		cout << "[ERROR] binding UDP in Socket" << endl;
	}
}

void NetworkManager::SetRemoteData()
{
	string receiverIP = "";
	int receiverPORT;
	cout << "Please enter your receipcient's IP: " << endl;
	cin >> receiverIP;
	cout << "Please enter your receipcient's PORT: " << endl;
	cin >> receiverPORT;

	UDPOutAddr.sin_family = AF_INET;
	UDPOutAddr.sin_port = htons((u_short)receiverPORT);
	cout << (u_short)receiverPORT << endl;
	//UDPOutAddr.sin_port = htons(8889);
	inet_pton(AF_INET, receiverIP.c_str(), &UDPOutAddr.sin_addr.s_addr);
	cout << receiverIP.c_str() << endl;
	//inet_pton(AF_INET, "127.0.0.1", &UDPOutAddr.sin_addr.s_addr);
	
}

void NetworkManager::CreateUDPSockets()
{
	cout << "NetworkManager::CreateUDPSockets()" << endl;

	//i.AF_INET: tell it we are using IPv4 
	//ii. SOCK_DGRAM: we are using UDP 
	//iii. IPPROTO_UDP: Any Protocol Available
	UDPSocketIn = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UDPSocketIn == INVALID_SOCKET)
	{
		cout << "UDP socket in failed to create!" << endl;
		ShutDown();
	}

	UDPSocketOut = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UDPSocketIn == INVALID_SOCKET)
	{
		cout << "UDP socket out failed to create!" << endl;
		ShutDown();
	}
}

void NetworkManager::SendDataUDP(const char* message)
{
	int totalBytesSent = sendto(UDPSocketOut, message, strlen(message) + 1, 0,
		reinterpret_cast<SOCKADDR*>(&UDPOutAddr), sizeof(UDPOutAddr));

	if (totalBytesSent == SOCKET_ERROR)
	{
		ShutDown();
	}

	cout << "sent: " << totalBytesSent << " of data. " << endl;
}

int NetworkManager::ReceiveDataUDP(char* message)
{
	//string portToReceive = "";
	//cout << "Please enter your PORT to receive the data: " << endl;
	//cin >> portToReceive;
	int byteReceived = 0;
	int inAddrSize = sizeof(UDPInAddr);

	byteReceived = recvfrom(UDPSocketIn, message, MAX_MSG_SIZE, 0,reinterpret_cast<SOCKADDR*>(&UDPInAddr),&inAddrSize);

	if (byteReceived == SOCKET_ERROR)
	{
		ShutDown();
	}
	return byteReceived;
}
