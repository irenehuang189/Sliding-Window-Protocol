/*
Tugas Besar 1 Jaringan Komputer
SLIDING WINDOW PROTOCOL
Dibuat oleh:
- Irene Wiliudarsan	(13513002)
- Muhammad Ridwan	(13513008)
- Angela Lynn		(13513032)
File : frame.h
*/ 

#ifndef _FRAME_H_
#define _FRAME_H_

/* ASCII Const */
#define SOH 1 /* Start of Header Character */
#define STX 2 /* Start of Text Character */
#define ETX 3 /* End of Text Character */
#define ENQ 5 /* Enquiry Character */
#define ACK 6 /* Acknowledgement */
#define BEL 7 /* Message Error Warning */
#define CR 13 /* Carriage Return */
#define LF 10 /* Line Feed */
#define NAK 21 /* Negative Acknowledgement */
#define Endfile 26 /* End of file character */
#define ESC 27 /* ESC key */

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

/* Const */
#define BYTESIZE 256 /* The maximum value of a byte */
#define MAXLEN 1024 /* Maximum messages length */

typedef unsigned char Byte;
typedef struct Frame {
	unsigned int soh;
	unsigned int frameNumber;
	unsigned int stx;
	Byte *data;
	unsigned int etx;
	Byte checkSum;
} Frame;
typedef struct {
	unsigned int ack;
	unsigned int frameNumber;
	Byte checkSum;
} Ack;

// Getter
unsigned int getSoh(Frame frame);
unsigned int getFrameNumber(Frame frame);
unsigned int getStx(Frame frame);
Byte* getData(Frame frame);
unsigned int getEtx(Frame frame);
unsigned int getCheckSum(Frame frame);
unsigned int getAck(Ack ack);
unsigned int getFrameNumber(Ack ack);
unsigned int getCheckSum(Ack ack);
Byte countCheckSum();

// Fungsi dan prosedur lainnya
bool isAckNak(unsigned int x);
bool isFrameValid(Frame frame);

// Setter
void setDataToFrame(Byte *data, unsigned int frameNumber, Frame *frame);
void setFrameToPointer(Frame frame, Byte *message);
void setPointerToFrame(Byte * message, Frame *frame);
void setAck(unsigned int ack, unsigned int frameNumber, Byte checkSum, Frame *frame);


#endif