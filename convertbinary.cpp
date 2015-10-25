#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#define ETX 3

int generateChecksum(char* checkString);
void printbit(long long a);

int main(int argc, char *argv[])
{
	//char * bin = convertstringtobinary("hello");
	//printf("%s\n",bin);
	//convert(bin);
	int checksum;
	char *msg = "Dd";
	//scanf("%s",&msg);
	printf("%s\n",msg);
	int MsgLen = strlen(msg);
	int size = MsgLen + 1;
	char ret[size]; 
	int i;
	for (i = 0; i < MsgLen; i++) {
		ret[i] = msg[i];	
	}
	char *etxstr;
	ret[MsgLen] = ETX;

	checksum = generateChecksum(ret);
	printf("%d\n",checksum);
    return 0;
}

int generateChecksum(char* stringFrame) {
	unsigned long long checksum = 0;
	unsigned long long CRC= 0x14B;
	int i=0;
	do{
		checksum = checksum <<8;
		checksum +=  (unsigned long long)  stringFrame[i];
		i++;
	}while(stringFrame[i]!= ETX);
	checksum = (checksum <<8);
	//printbit(checksum);
	for(int i = sizeof(long long)*8; i>8; i--){
		if((checksum>>(i-1))&1){
			checksum = checksum ^ (CRC<<(i-9));
			//printbit(CRC<<(i-9));
			//printbit(checksum);
		}
	}
	return (int) checksum;
}

void printbit(long long a){
	int i;
	for(i= sizeof(long long)*8; i>0; i--){
		printf("%d", (int)((a>>(i-1))&1));
	}
	printf("\n\n");
}
