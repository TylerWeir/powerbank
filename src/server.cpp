/* server.cpp
 *
 * This server recieves floats via tcp connection and records them
 * into a log file.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#include <stdio.h>
#include <string.h>

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


	fd_set master;
	FD_ZERO(&master);
	FD_SET(socket_listen, &master);
	SOCKET max_socket = socket_listen;


	printf("Waiting for connection...\n");

	while(1) {
		// Copy master into reads since select modifies the set given to it
		fd_set reads;
		reads = master;
		if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
			fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
			return 1;
		}

		// Loop through the sockets to see if they were flagged by select as
		// being ready to read
		SOCKET i;
		for (i = 1; i <= max_socket; i++) {
			if (FD_ISSET(i, &reads)) {

				// If the listening socket is marked as ready, that means it is 
				// ready to accept a new connection.
				if (i == socket_listen) {
					struct sockaddr_storage client_address;
					socklen_t client_len  = sizeof(client_address);
					SOCKET socket_client = accept(socket_listen, 
							(struct sockaddr*) &client_address, &client_len);

					if (!ISVALIDSOCKET(socket_client)) {
						fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
						return 1;
					}

					FD_SET(socket_client, &master);
					if (socket_client > max_socket)
						max_socket = socket_client;

					char address_buffer[100];
					getnameinfo((struct sockaddr*)&client_address, client_len, 
							address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
					printf("New connection from %s\n", address_buffer);
				} else {
					// If the ready socket is not the listening socket, then there is 
					// some data to read.
					char read[1024];
					int bytes_received = recv(i, read, 1024, 0);

					// Close the socket if it sends an empty request
					if (bytes_received < 1) {
						FD_CLR(i, &master);
						printf("Closing client connection.\n");
						CLOSESOCKET(i);
						continue;
					}

					int j;
					for (j = 0; j < bytes_received; ++j)
						printf("%c", read[j]);
					// TODO tmp echo response
					send(i, read, bytes_received, 0);
				}
			}
		}
	}

	printf("Finished.\n");
	return 0;
}


