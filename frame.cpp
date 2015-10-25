/*
Tugas Besar 1 Jaringan Komputer
SLIDING WINDOW PROTOCOL
Dibuat oleh:
- Irene Wiliudarsan	(13513002)
- Muhammad Ridwan	(13513008)
- Angela Lynn		(13513032)
File : frame.cpp
*/ 

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "frame.h"

using namespace std;

// Getter
unsigned int getSoh(Frame frame) {
	return frame.soh;
}
unsigned int getFrameNumber(Frame frame) {
	return frame.frameNumber;
}
unsigned int getStx(Frame frame) {
	return frame.stx;
}
char* getData(Frame frame) {
	return frame.data;
}
unsigned int getEtx(Frame frame) {
	return frame.etx;
}
unsigned int getCheckSum(Frame frame) {
	return frame.checkSum;
}
unsigned int getAck(Ack ack) {
	return ack.ack;
}
unsigned int getFrameNumber(Ack ack) {
	return ack.frameNumber;
}
unsigned int getCheckSum(Ack ack) {
	return ack.checkSum;
}

// Fungsi dan prosedur lainnya
bool isAckNak(unsigned int x) {
	return (x == ACK);
}
bool isFrameValid(Frame frame) {
	return true;
	// getCheckSum() == countCheckSum()
}
bool isFrameEmpty(Frame frame) {
	return ((frame.soh==EMPTY) && (frame.frameNumber==EMPTY) && (frame.stx==EMPTY) && (frame.data==NULL) && (frame.etx==EMPTY) && frame.checkSum==EMPTY);
}
int countCheckSum() {
	return 0;
}

// Setter
void setEmptyFrame(Frame frame) {
	frame.soh = EMPTY;
	frame.frameNumber = EMPTY;
	frame.stx = EMPTY;
	frame.data = NULL;
	frame.etx = EMPTY;
	frame.checkSum = EMPTY;
}
void setDataToFrame(char *data, unsigned int frameNumber, Frame &frame) {
	frame.soh = SOH;
	frame.frameNumber = frameNumber;
	frame.stx = STX;
	frame.data = data;
	frame.etx = ETX;
	frame.checkSum = countCheckSum();
}
void setFrameToPointer(Frame frame, char *message) {
	char temp[100];
	sprintf(message, "%d||%d||%d||", frame.soh, frame.frameNumber, frame.stx);
	char tempChar[sizeof(frame.data)];
	int i = 0;
	while (frame.data[i] != '\0') {
		tempChar[i] = frame.data[i];
		i++;
	}
	strcat(message, tempChar);
	sprintf(temp, "||%d||%d||", frame.etx, frame.checkSum);
	strcat(message, temp);
}
void setPointerToFrame(char *message, Frame &frame) {
	int i = 0, iStart = 0;
	char temp[strlen(message)];
	for (int nPartition = 0; nPartition < 6; nPartition++) {
		while (message[i] == '|' && message[i+1] == '|') {
			i++;
		}
		// Copy char to frame
		for (int j = iStart; j < i; j++) {
			temp[strlen(message)] = message[i];
			temp[strlen(message)+1] = '\0';
		}
		if (nPartition == 0) {
			// SOH
			frame.soh = atoi(temp);
		} else if (nPartition == 1) {
			// frameNumber
			frame.frameNumber = atoi(temp);

		} else if (nPartition == 2) {
			// STX
			frame.stx = atoi(temp);
		} else if (nPartition == 3) {
			// Data
			strcpy(frame.data, temp);
		} else if (nPartition == 4) {
			// ETX
			frame.etx = atoi(temp);
		} else {
			// checkSum
			frame.checkSum = temp[0];
		}
		temp[0] = '\0';
		//temp = message.substr(iStart, i-iStart);
		iStart = i+2;
	}
}
void setAck(unsigned int ackValue, unsigned int frameNumber, int checkSum, Ack &ack) {
	ack.ack = ackValue;
	ack.frameNumber = frameNumber;
	ack.checkSum = checkSum;
}
void setAckToPointer(Ack ack, char *message) {
	sprintf(message, "%d||%d||%d||", ack.ack, ack.frameNumber, ack.checkSum);
}
void setPointerToAck(char *message, Ack ack) {
	int i = 0, iStart = 0;
	char temp[strlen(message)];
	for (int nPartition = 0; nPartition < 3; nPartition++) {
		while (message[i] == '|' && message[i+1] == '|') {
			i++;
		}
		// Copy char to ack
		for (int j = iStart; j < i; j++) {
			temp[strlen(message)] = message[i];
			temp[strlen(message)+1] = '\0';
		}
		if (nPartition == 0) {
			ack.ack = atoi(temp);
		} else if (nPartition == 1) {
			ack.frameNumber = atoi(temp);
		} else {
			ack.checkSum = atoi(temp);
		}
		iStart = i+2;
	}
}
