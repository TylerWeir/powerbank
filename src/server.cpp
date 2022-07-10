/* server.cpp
 *
 * This server recieves floats via tcp connection and records them
 * into a log file.
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

int main() {

	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);


	printf("Creating socket...\n");
	SOCKET socket_listen;
	socket_listen = socket(bind_address->ai_family,
						   bind_address->ai_socktype,
						   bind_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}


	printf("Binding socket to local address...\n");
	if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	freeaddrinfo(bind_address);


	printf("Listening...\n");
	if (listen(socket_listen, 10) < 0 ) {
		fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}


	printf("Waiting for connection...\n");
	struct sockaddr_storage client_address;
	socklen_t client_len = sizeof(client_address);
	SOCKET socket_client = accept(socket_listen,
			(struct sockaddr*) &client_address, &client_len);
	if (!ISVALIDSOCKET(socket_client)) {
		fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	while(1) {
		printf("Client is connected... ");
		char address_buffer[100];
		getnameinfo((struct sockaddr*) &client_address,
				client_len, address_buffer, sizeof(address_buffer), 0, 0,
				NI_NUMERICHOST);
		printf("%s\n", address_buffer);


		printf("Reading request...\n");
		char request[1024];
		int bytes_received = recv(socket_client, request, 1024, 0);
		printf("Received %d bytes.\n", bytes_received);


		printf("Sending response...\n");
		const char *response = "General Kenobi.";
		int bytes_sent = send(socket_client, response, strlen(response), 0);
		printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

	}	
}

