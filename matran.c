#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>

#define spi0 0

unsigned char buf[2];
//unsigned char mssv[8]={2,0,1,4,6,5,1,0};
unsigned char mssv[7]={17,23,28,16,94,6,111};
unsigned char led8x8[8]={0x00,0x66,0x66,0x00,0x81,0x42,0x3c,0xA1};
// c,o,v,i,d,1,9
//{c,o,v,i,d,1,9}
void sendData(unsigned char address, unsigned char data ){
buf[0] = address;
buf[1] = data;
wiringPiSPIDataRW(spi0,buf,2);
}

void init(void){
//decode mode 
sendData(0x09,0x00);

sendData(0x0A,0x9);
// instensity - độ sáng
sendData(0x0A,0x01);
//scan limit 8 số
sendData(0x0B,0x07);
sendData(0x0C,1);
sendData(0x0F,0);
}

int main(void){

wiringPiSPISetup(spi0,10000000);
init();
// sendData(i+1,mssv[6-i]);
for(int i=0;i<=7;i++){
//sendData(i+1,mssv[6-i]);
sendData(i+1,led8x8[7-i]);
}

return 0;
}
