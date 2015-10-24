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
bool isAck(unsigned int x) {
	return (x == ACK);
}
bool isFrameValid(Frame frame) {
	return true;
	// getCheckSum() == countCheckSum()
}
char countCheckSum() {
	return 'a';
}

// Setter
void setDataToFrame(char *data, unsigned int frameNumber, Frame &frame) {
	frame.soh = SOH;
	frame.frameNumber = frameNumber;
	frame.stx = STX;
	frame.data = data;
	frame.etx = ETX;
	frame.checkSum = countCheckSum();
}
void setFrameToPointer(Frame frame, char *message) {
	cout << "test" << endl;
	char temp[100];
	sprintf(temp, "%d", frame.soh);
	strcat(message, temp);
	sprintf(temp, "%d", frame.frameNumber);
	strcat(message, temp);
	sprintf(temp, "%d", frame.stx);
	strcat(message, temp);
	char tempChar[sizeof(frame.data)];
	for (int i=0; i<sizeof(frame.data)-1; i++) {
		tempChar[i] = frame.data[i];
		strcat(message, tempChar);
	}
	sprintf(temp, "%d", frame.etx);
	strcat(message, temp);
	sprintf(temp, "%c", frame.checkSum);
	strcat(message, temp);
}
void setPointerToFrame(char *message, Frame &frame) {

}
void setAck(unsigned int ackValue, unsigned int frameNumber, char checkSum, Ack &ack) {
	ack.ack = ackValue;
	ack.frameNumber = frameNumber;
	ack.checkSum = checkSum;
}

