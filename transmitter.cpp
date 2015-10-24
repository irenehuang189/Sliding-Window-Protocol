#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "frame.h"

using namespace std;

char data[100];

void initiate(char *server, char *servPort);
void readFile(char *fileinput);
void DieWithSystemMessage(const char *msg);
void DieWithUserMessage(const char *msg, const char *detail);

int main(int argc, char *argv[]){
	int i = 0;
	initiate(argv[1],argv[2]);
	//char *fileinput = "tes.txt";
	readFile(argv[3]);
	while (data[i] != '\0'){
		cout << data[i];
		i++;
	}
	return 0;
}

void initiate(char *server, char *servPort){
	int sock;
	// address info
	struct addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));
	addrCriteria.ai_family = AF_UNSPEC;

	addrCriteria.ai_socktype = SOCK_DGRAM;
	addrCriteria.ai_protocol = IPPROTO_UDP;
	
	struct addrinfo *servAddr; // List of server addresses
	int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);
	if (rtnVal != 0)
	DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
	// Create a datagram/UDP socket
	sock = socket(servAddr->ai_family, servAddr->ai_socktype,
	servAddr->ai_protocol); // Socket descriptor for client
	if (sock < 0)
	//printf("socket() failed");
	DieWithSystemMessage("socket() failed");
}

void readFile(char *fileinput){
	//openfile
	FILE *fp;
	char str[80];
	int i,j,k;
	fp = fopen(fileinput, "r");
	j = 0;
	k = 0;
	do {
		fgets(str, 80, fp);
		i = 0;
		k = j;
		while(str[i] != '\0' && str[i] != '\n') {
			data[j] = str[i];
			i++;
			j++;
		}
		if (str[i] == '\n') {
			data[j] = '\n';
			j++;
		}
	} while (!feof(fp));
	data[k] = '\0';
	fclose(fp);
}

void DieWithUserMessage(const char *msg, const char *detail) {
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DieWithSystemMessage(const char *msg) {
	perror(msg);
	exit(1);
}
