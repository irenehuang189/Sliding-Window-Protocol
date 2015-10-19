/*
Tugas Besar 1 Jaringan Komputer
SLIDING WINDOW PROTOCOL
Dibuat oleh:
- Irene Wiliudarsan	(13513002)
- Muhammad Ridwan	(13513008)
- Angela Lynn		(13513032)
File : frame.cpp
*/ 

#include "frame.h"

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
Byte* getData(Frame frame) {
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
Byte countCheckSum() {
	return 'a';
}

// Setter
void setDataToFrame(Byte *data, unsigned int frameNumber, Frame *frame) {
	(*frame).soh = SOH;
	(*frame).frameNumber = frameNumber;
	(*frame).stx = STX;
	(*frame).data = data;
	(*frame).etx = ETX;
	(*frame).checkSum = countCheckSum();
}
void setFrameToPointer(Frame frame, Byte *message) {
	
}
void setPointerToFrame(Byte * message, Frame *frame) {

}
void setAck(unsigned int ack, unsigned int frameNumber, Byte checkSum, Frame *frame) {

}

