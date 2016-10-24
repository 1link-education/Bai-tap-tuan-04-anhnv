#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "listener.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_RECVPORT "27015"
#define DEFAULT_SENDPORT "27016"

using namespace std;

void tcp_recv(/*char recvbuf[DEFAULT_BUFLEN]*/) 
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN+1];
	//char * recvbuf;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("\n\tWSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_RECVPORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("\n\tgetaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("\n\tsocket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("\n\tbind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	// Listening for client socket
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("\n\tlisten failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("\n\taccept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("\n\tbytes received: %d", iResult);

			string raw_str_handle;
			int i;
			for(i = 0; i < iResult; i++)												// char[] to string
				if(recvbuf[i] != NULL)
					raw_str_handle += recvbuf[i];
			size_t ext = raw_str_handle.find_first_of(":");
			if(ext == std::string::npos)
				listen_for_schl(raw_str_handle);
			else
				server_listener(recvbuf, iResult);

			// Echo the buffer back to the sender				-> is this TCP?
			iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
			if (iSendResult == SOCKET_ERROR) {
				printf("\n\tsend failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return;
			}
			printf("\n\tbytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("\n\tconnection closing...\n");
		else  {
			printf("\n\trecv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("\n\tshutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

void tcp_send(char *sendbuf)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//char *sendbuf = "this is a test";		// "'this is a test' is 14 Bytes long"
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("\n\tWSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", DEFAULT_SENDPORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("\n\tgetaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("\n\tsocket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("\n\tunable to connect to server!\n");
		WSACleanup();
		return;
	}

	// Send an initial buffer
	iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );						
	// buf(sendbuf) must be const char (A pointer to a buffer containing the data to be transmitted) :-( fuck

	if (iResult == SOCKET_ERROR) {
		printf("\n\tsend failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	printf("\n\tbytes sent: %ld", iResult);



	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("\n\tshutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 )
			printf("\n\tbytes received: %d\n", iResult);
		else if ( iResult == 0 )
			printf("\n\tconnection closed\n");
		else
			printf("\n\trecv failed with error: %d\n", WSAGetLastError());

	} while( iResult > 0 );

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}