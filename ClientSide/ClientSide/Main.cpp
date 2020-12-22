#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <conio.h>

#pragma comment(lib,"Ws2_32.lib")

using namespace std;

void main()
{
	WSADATA wsaData;

	// NOTE: Try to initialize WinSock library
	int initializeOutput = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// NOTE: Intialize errors handling
	if (initializeOutput != 0) {
		printf("WSAStartup failed with error: %d\n", initializeOutput);
		return;
	}

	// NOTE: Define socket with TCP protocol
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// NOTE: Socket params
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_port = htons(8000);
	ad.sin_addr.s_addr = inet_addr("127.0.0.1");
	int adSize = sizeof(ad);
	

	// NOTE: Try to connect to the server
	int connectionResult = connect(connectSocket, (struct sockaddr*)&ad, adSize);
	if (connectionResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		cout << "Error while connecting socket\n";
		return;
	}

	cout << "You was successfully connected to the server" << endl << endl;
	cout << "This program will generate weather forecast for selected day of week" << endl;
	
	int input = 0;
	while (input != 8) {

		fflush(stdout);

		// NOTE: UI
		cout << endl << "Please, enter the day of week (from 1 to 7) or 8 to exit" << endl;
		input = _getche();

		// NOTE: From 1 to 8
		if (input < 49 || input > 56) {
			cout << endl << "** Please, input correct number **" << endl;
			continue;
		}

		// NOTE: Exit
		if (input == 56) {
			break;
		}

		// NOTE: Prepare request buffer
		char inputBuffer[1024];
		sprintf_s(inputBuffer, "%d", input);

		// NOTE: Send request
		send(connectSocket, inputBuffer, sizeof(inputBuffer), 0);

		// NOTE: Receive response
		char responseBuffer[1024];
		int response = recv(connectSocket, responseBuffer, sizeof(responseBuffer), 0);

		if (response == SOCKET_ERROR) {
			int iError = WSAGetLastError();
			if (iError == WSAEWOULDBLOCK)
				printf("\nrecv failed with error : WSAEWOULDBLOCK\n");
			else
				printf("\nrecv failed with error code: %ld\n", iError);

			continue;
		}

		responseBuffer[response] = '\0';
		cout << endl << "\nServer response:\n" << responseBuffer << endl;
	}

	// NOTE: Close socket
	closesocket(connectSocket);

	// NOTE: Dispose WinSock lib
	WSACleanup();
}