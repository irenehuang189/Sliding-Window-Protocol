#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <thread>
#include <arpa/inet.h>
#include <algorithm>
#include "frame.h"

using namespace std;

struct sockaddr_in receiverAddress; // Alamat
socklen_t receiverLength = sizeof(receiverAddress);
int sockfd; // Socket
int windowHead = 0;	// variabel yang menyimpan nomor frame yang terletak di window paling depan
int window[WINDOWSIZE] = {0};	// array of int yang menampung status frame pada window. 0 artinya perlu dikirim, -1 artinya menunggu timeout, 1 artinya sudah ACK
char data[100];	// Array of char yang menampung karakter dari file
int nData = 0;	// Jumlah elemen pada array of char data
Frame *frames;	// array of Frame yang menampung semua frame yang dimiliki transmitter. frames[0] berisi frame dengan nomor 1, dst
int frameNumber = 0;	// Banyaknya frame
bool endFrame = false;	// True jika semua frame yang ada sudah ACK

void initiate(char *server, char *servPort);
void initiate2(char *server, char *servPort);
void readFile(char *fileinput);
void DieWithSystemMessage(const char *msg);
void DieWithUserMessage(const char *msg, const char *detail);
void splitDataToFrames();
void sendFrame();
void receiveAckNak();
void waitTimeOut();

int main(int argc, char *argv[]){
	// int i = 0;
	initiate2(argv[1],argv[2]);
	//char *fileinput = "tes.txt";
	readFile(argv[3]);
	/*while (data[i] != '\0'){
		cout << data[i];
		i++;
	}*/

	frames = new Frame [nData];
	splitDataToFrames();

	// Membuat dan mensinkronisasikan thread
	thread thread1 (sendFrame);
	thread thread2 (receiveAckNak);
	thread1.join();
	thread2.join();

	delete [] frames;
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

void initiate2(char *server, char *servPort) {
	// Membuat UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		cout << "Socket tidak dapat dibuat" << endl;
		exit(1);
	} else {
		cout << "Membuat socket untuk koneksi ke " << server << ":" << servPort << " ..." << endl;
	}

	// Alamat socket
	struct sockaddr_in transmitterAddress;
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_port = htons(atoi(servPort));
	receiverAddress.sin_addr.s_addr = inet_addr(server);
	memset(receiverAddress.sin_zero,'\0',sizeof receiverAddress.sin_zero);
	if (transmitterAddress.sin_addr.s_addr == INADDR_NONE) {
		cout << "IP Address tidak ada" << endl;
		exit(1);
	}
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
			nData++;
			i++;
			j++;
		}
		/*if (str[i] == '\n') {
			data[j] = '\n';
			j++;
		}*/
	} while (!feof(fp));
	// data[k] = '\0';
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

void splitDataToFrames() {
	int i = 0;

	while (i < nData) {
		if ((nData - i) < DATALENGTH) {
			char tempData[DATALENGTH];
			for (int j = 0; j < DATALENGTH; j++) {
				tempData[j] = data[i];
				i++;
			}
			setDataToFrame(tempData, frameNumber, frames[frameNumber]);
			frameNumber++;
		} else {
			char tempData[nData-i];
			int n;

			n = nData - i;
			for (int j = 0; j < n; j++) {
				tempData[j] = data[i];
				i++;
			}
			setDataToFrame(tempData, frameNumber, frames[frameNumber]);
			frameNumber++;
		}
	}
}

void sendFrame() {
	bool endWindow = false;

	cout << "Window berada di Frame " << windowHead << " - " << windowHead+WINDOWSIZE-1 << endl;
	while (!endFrame) {
		for (int i = 0; i < WINDOWSIZE; i++) {
			// Jika frame belum dikirim
			if (window[i] == 0) {
				char message[100];
				setFrameToPointer(frames[windowHead+i], message);
				sendto(sockfd, message, 100, 0, (struct sockaddr *) &receiverAddress, receiverLength);
				cout << "Mengirim Frame ke " << windowHead+i+1 << ": " << getData(frames[windowHead+i+1]) << endl;
			}

			// Looping lagi dari awal window
			if (i == (WINDOWSIZE-1)) {
				i = -1;
			}
		}

		// Jika window sudah tidak bisa di-slide
		if (endWindow) {
			// Jika semua frame di window sudah ACK, maka selesai
			endFrame = all_of(begin(window), end(window), [](int w){return w==1;});
		} else {
			// Jika frame paling awal di window sudah ACK, maka slide
			if (window[0] = 1) {
				if ((windowHead+WINDOWSIZE) > (frameNumber-1)) {
					endWindow = true;
				} else {
					windowHead++;
					cout << "Window berada di Frame " << windowHead << " - " << windowHead+WINDOWSIZE-1 << endl;
				}
			}
		}
	}
}

void receiveAckNak() {
	char ackChar[100];
	int rec;
	Ack receiverAck;

	while (!endFrame) {
		rec = recvfrom(sockfd, ackChar, 100, 0, (struct sockaddr *) &receiverAddress, (socklen_t *) &receiverLength);
		if (rec < 0) {
			perror("Pembacaan data dari receiver tidak berhasil");
		}

		setPointerToAck(ackChar, receiverAck);
		if (isAckNak(getAck(receiverAck))) {
			window[getAck(receiverAck) - windowHead] = 1;
			cout << "Frame " << getAck(receiverAck)-windowHead << " ACK" << endl;
		} else { // NAK
			window[getAck(receiverAck) - windowHead] = 0;
			cout << "Frame " << getAck(receiverAck)-windowHead << " NAK" << endl;
		}
	}
}

void waitTimeOut() {
	while (!endFrame) {

	}
}