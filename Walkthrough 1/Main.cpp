#include "iostream"
#include "NetworkManager.h"
#include <string>
#include <conio.h>

using namespace std;

int main()
{
	cout << "helllo world" << endl;

	NetworkManager::GetInstance()->Init();

	//NetworkManager* NetworkInst = NetworkManager::GetInstance();
	//NetworkInst->Init();

	NetworkManager::GetInstance()->CreateUDPSockets();
	NetworkManager::GetInstance()->BindUDP();
	NetworkManager::GetInstance()->SetRemoteData();
	string receiverIP = "";
	string receiverPORT = "";
	string portToReceive = "";
	//cout << "Please enter your receipcient's IP: " << endl;
	//cin >> receiverIP;
	//cout << "Please enter your receipcient's PORT: " << endl;
	//cin >> receiverPORT;
	//cout << "Please enter your PORT to receive the data: " << endl;
	//cin >> portToReceive;
	

	//Major:
	string sendString = "";
	sendString[0] = '\0'; //end of the string
	cout << "Press q to exit/quit or type the message to send: " << endl;
	while (sendString != "q") 
	{
		//cout << "Press q to exit/quit or type the message to send: " << endl;
		//cout << "[Press q to exit/quit]" << endl; Please enter a message to send over the network!!!:

		if (_kbhit())
		{
			getline(cin, sendString); //need to include string
		}
		if (sendString != "q" && sendString[0] != '\0')
		{
			cout << "Sent message: " << sendString << endl;
			NetworkManager::GetInstance()->SendDataUDP(sendString.c_str()); //char by char

			char* recString = new char[NetworkManager::GetInstance()->MAX_MSG_SIZE];
			recString[0] = '\0';

			int size = NetworkManager::GetInstance()->ReceiveDataUDP(recString);

			if (size > 0)
			{
				cout << "Received message: " << recString << endl;
			}
			sendString[0] = '\0';
		}
	}

	

	NetworkManager::GetInstance()->ShutDown();
	
	return 0;
}