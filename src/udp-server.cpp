/*udp-server.cpp*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>


int main() {

	std::cout << "Configuring local address...\n";
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);
	
	
	std::cout << "Creating socket...\n";
	int socket_listen;
	socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (socket < 0) {
		std::cerr << "socket() failed. (" << errno << ")\n";
		return 1;
	}


	std::cout << "Binding socket to local address...\n";
	if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
		std::cerr << "bind() failed. (" << errno << ") \n";
		return 1;
	}
	freeaddrinfo(bind_address);


	struct sockaddr_storage client_address;
	socklen_t client_len = sizeof(client_address);
	char read[1024];
	int bytes_received = recvfrom(socket_listen, read, 1024, 0,
		(struct sockaddr*) &client_address, &client_len);
	std::cout << "Received " << bytes_received << "bytes.\n";


	close(socket_listen);
	std::cout << "Finished.\n";
	return 0;

}
