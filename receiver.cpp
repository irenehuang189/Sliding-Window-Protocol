#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "frame.h"
#include "stdbool.h"

using namespace std;

/* Socket */
int sockfd; // listen on sock_fd

/* Functions declaration */
void initiate(char *servPort);

int main(int argc, char *argv[])
{
	initiate(argv[1]);
	return 0;
}

void initiate(char *servPort){
	// Construct the server address structure
	struct addrinfo addrCriteria;						// Criteria for address
	memset(&addrCriteria, 0, sizeof(addrCriteria));		// Zero out structure
	addrCriteria.ai_family = AF_UNSPEC;					// Any address family
	addrCriteria.ai_flags = AI_PASSIVE;					// Accept on any address/port
	addrCriteria.ai_socktype = SOCK_DGRAM;				// Only datagram socket
	addrCriteria.ai_protocol = IPPROTO_UDP;				// Only UDP socket
	
	struct addrinfo *servAddr; // List of server addresses
	int rtnVal = getaddrinfo(NULL, servPort, &addrCriteria, &servAddr);
	if (rtnVal != 0) {
		cout << "getaddrinfo() failed" << endl;
	}
	
	// Buat socket
	sockfd = socket(servAddr->ai_family, servAddr->ai_socktype,
	servAddr->ai_protocol);
	if (sockfd < 0) {
		cout << "socket() failed" << endl;
	}
	
	// Bind ke local address
	if (bind(sockfd, servAddr->ai_addr, servAddr->ai_addrlen) < 0) {
		cout << "bind() failed" << endl;
	} else {
		struct ifaddrs *ifAddrStruct = NULL;
		void * tmpAddrPtr = NULL;
		
		getifaddrs(&ifAddrStruct);
		struct ifaddrs *ifa = ifAddrStruct;
		ifa = ifa->ifa_next;
		ifa = ifa->ifa_next;
		ifa = ifa->ifa_next;
		tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
		char addressBuffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
		cout << "Binding pada " << addressBuffer << endl;
		
		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		if (getsockname(sockfd, (struct sockaddr *)&sin, &len) != -1)
			cout << ntohs(sin.sin_port) << "..." << endl;
	}
}
