#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <string>

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
	ad.sin_addr.s_addr = 0;
	int adSize = sizeof(ad);

	// NOTE: Bind params to socket
	bind(connectSocket, (struct sockaddr*)&ad, sizeof(ad));
	
	// NOTE: Start to listen requests
	listen(connectSocket, 5);

	while (1) {

		sockaddr_in remoteAd;
		int remoteAdSize = sizeof(remoteAd);

		printf("Waiting for clients\n");

		// NOTE: Receive connections
		SOCKET personalSocket = accept(connectSocket, (struct sockaddr*)&remoteAd, &remoteAdSize);
		if (personalSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			continue;
		}

		printf("\nCreated connection with client\n");

		char dataBuffer[1024];
		while (recv(personalSocket, dataBuffer, sizeof(dataBuffer), 0) != 0) {

			printf("Received packet from %s:%d\n", inet_ntoa(remoteAd.sin_addr), ntohs(remoteAd.sin_port));
			printf("Data: %s\n", dataBuffer);

			char response[1024];

			int choice = atoi(dataBuffer);
			switch (choice) {
			case 49:
				strcpy(response, "Monday:\nDay: Sunshine and clouds mixed. High 46F. Winds WNW at 10 to 15 mph.\nNight: Mostly clear. Low 32F. Winds WNW at 10 to 15 mph.");
				break;
			case 50:
				strcpy(response, "Tuesday:\nDay: Sunny along with a few clouds. High near 40F. Winds light and variable.\nNight: Mostly cloudy skies. Low 38F. Winds S at 5 to 10 mph.");
				break;
			case 51:
				strcpy(response, "Wednesday:\nDay: Cloudy in the morning, then off and on rain showers during the afternoon hours. High 58F. Winds SSE at 15 to 25 mph. Chance of rain 40%. Higher wind gusts possible.\nNight: Windy with rain likely. Potential for heavy rainfall. Low 49F. Winds SSE at 20 to 30 mph. Chance of rain 80%. 1 to 2 inches of rain expected.");
				break;
			case 52:
				strcpy(response, "Thursday:\nDay: Rain showers early with some sunshine later in the day. Morning high of 55F with temps falling sharply to near 35. Winds SW at 10 to 20 mph. Chance of rain 50%.\nNight: Mainly clear skies. Low near 25F. Winds W at 10 to 20 mph.");
				break;
			case 53:
				strcpy(response, "Friday:\nDay: Sunny skies. High 33F. Winds W at 10 to 15 mph.\nNight: Clear. Low 27F. Winds W at 5 to 10 mph.");
				break;
			case 54:
				strcpy(response, "Saturday:\nDay: Except for a few afternoon clouds, mainly sunny. High 38F. Winds light and variable.\nNight: Partly to mostly cloudy. Low 32F. Winds light and variable.");
				break;
			case 55:
				strcpy(response, "Sunday:\nDay: Cloudy skies with periods of rain later in the day. High 49F. Winds SE at 5 to 10 mph. Chance of rain 60%.\nNight: A steady rain in the evening. Showers continuing late. Low 36F. Winds W at 5 to 10 mph. Chance of rain 70%. Rainfall near a half an inch.");
				break;
			default:
				strcpy(response, "Received incorrect data");
				break;
			}

			send(personalSocket, response, strlen(response), 0);
		}

		closesocket(personalSocket);
	}

	// NOTE: Close socket
	closesocket(connectSocket);

	// NOTE: Dispose WinSock lib
	WSACleanup();
}

