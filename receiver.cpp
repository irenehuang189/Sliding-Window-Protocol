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
#include <thread>
#include "frame.h"
#include "stdbool.h"

using namespace std;

/* Socket */
int sockfd; // listen on sock_fd
struct sockaddr_in receiverAddress, remoteAddress; // Alamat 
socklen_t addrlen = sizeof(remoteAddress);
Frame buffer[BUFFERSIZE];
Frame queue[BUFFERSIZE];
int iBuffer = 0;

/* Functions declaration */
void initiate(char *servPort);
void initiate2(char *portNumber);
void receiveFrame();
void consumeFrame();
void sendAckNak(unsigned int ackValue, Frame frame);
void moveQueueToBuffer();

int main(int argc, char *argv[])
{
	// Memeriksa parameter
	if (argc < 2) {
		cout << "Error, tidak ada port yang disediakan" << endl;
		exit(1);
	}
	initiate2(argv[1]);

	// Inisiasi isi queue
	for (int i = 0; i < BUFFERSIZE; i++) {
		setEmptyFrame(queue[i]);
	}

	thread parentThread(receiveFrame);
	thread childThread(consumeFrame);
	parentThread.join();
	childThread.join();

	close(sockfd);
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
	sockfd = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
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

void initiate2(char *portNumber) {
	// Membuat UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		// Socket bernilai -1 jika gagal dibuat
		cout << "Socket tidak dapat dibuat" << endl;
		exit(1);
	}

	// Bind socket ke alamat IP dan port
	int portNo = atoi(portNumber);
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_port = htons(portNo);
	receiverAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&receiverAddress, sizeof(receiverAddress)) < 0) {
		cout << "Binding gagal" << endl;
		close(sockfd);
		exit(1);
	} else {
		cout << "Binding pada " << INADDR_ANY << ":" << portNo << endl;
	}
}

void receiveFrame() {
	// Kamus Lokal
	int recvlen; // Jumlah bytes yang diterima
	char data[MAXLEN]; // Data dari transmitter
	int nReceived = 0; // Jumlah byte yang diterima
	Frame frame;

	// Algoritma
	recvlen = recvfrom(sockfd, data, strlen(data), 0, (struct sockaddr *) &remoteAddress, (socklen_t *) &addrlen);
	cout << data << endl;
	while (true) {
		// Ubah pesan menjadi frame
		// setPointerToFrame(data, frame);
		// // Periksa apakah data valid
		// if (isFrameValid(frame)) {
		// 	// Kirim ACK
		// 	sendAckNak(ACK, frame);
		// 	cout << "ACK frame ke-" << getFrameNumber(frame) << " dikirim" << endl;
		// 	if (iBuffer == getFrameNumber(frame)) {
		// 		buffer[iBuffer] = frame;
		// 		// Increment indeks buffer
		// 		if (iBuffer == WINDOWSIZE) {
		// 			iBuffer = 0;
		// 		} else {
		// 			iBuffer++;
		// 		}
		// 	} else {
		// 		queue[getFrameNumber(frame)] = frame;
		// 	}
		// 	moveQueueToBuffer();
		// } else {
		// 	sendAckNak(NAK, frame);
		// }
		// nReceived++;
		recvlen = recvfrom(sockfd, data, strlen(data), 0, (struct sockaddr *) &remoteAddress, (socklen_t *) &addrlen);
		cout << data << endl;
	}
	// Mengirim sinyal bahwa program telah berakhir
}

void consumeFrame() {
	while (true) {
		if (!isFrameEmpty(buffer[iBuffer])) {
			cout << "Frame ke-" << iBuffer << " dikonsumsi: " << getData(buffer[iBuffer]) << endl;
			setEmptyFrame(buffer[iBuffer]);
			// Increment indeks buffer
			if (iBuffer == WINDOWSIZE) {
				iBuffer = 0;
			} else {
				iBuffer++;
			}
		}
	}
}

void sendAckNak(unsigned int ackValue, Frame frame) {
	Ack ackMessage;
	char message[MAXLEN];
	setAck(ackValue, getFrameNumber(frame), getCheckSum(frame), ackMessage);
	setAckToPointer(ackMessage, message);
	sendto(sockfd, message, strlen(message), 0, (struct sockaddr *) &remoteAddress, addrlen);
}

void moveQueueToBuffer() {
	// Cari indeks di queue yang terisi
	int i = 0;
	bool isFound = false;
	while (!isFound && (i < BUFFERSIZE)) {
		if (isFrameEmpty(queue[i])) {
			isFound = true;
		} else {
			i++;
		}
	}
	if (iBuffer == i) {
		// Pindahkan isi queue ke buffer
		while (isFrameEmpty(queue[i]) && (i < BUFFERSIZE)) {
			buffer[i] = queue[i];
			setEmptyFrame(queue[i]);
			i++;
		}
	}
}
