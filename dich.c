#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#define spi0   0
uint8_t buf[2];
char dich[8]={0xFE, 0xFC, 0xFB, 0xF7, 0xEF, 0xCF, 0xBF, 0x7F};
int chuc, donvi,degree;
void sendData(uint8_t address, uint8_t data){
    buf[0] = address;
    buf[1] = data;
    wiringPiSPIDataRW(spi0, buf, 2);
}


void display(int temp, int i){
sendData(0x09,dich[i]);
degree=i+1;
chuc=i+3;
donvi=i+2;
if (chuc>8)
chuc=i+3-8;
if (donvi>8)
donvi=i+2-8;
for(int j=1; j<=8;j++)
if(j!=degree && j!=chuc && j!=donvi){
if (i==1)
sendData(1,0x00);
if (j==5 && i==5)
{
sendData(5,0x00);
}
else
sendData(j,0x0F);
}
sendData(degree,0x63);
sendData(chuc,temp/10);
sendData(donvi,temp%10);
printf("%d\n",i);
}




void Init(){
    // set decode mode: 0x09
    sendData(0x09,0xFE);
    // set intensity: 0x0A09
    sendData(0x0A, 9);
    // scan limit: 0x0B07
    sendData(0x0B, 7);
    // no shutdown, turn off display test
    sendData(0x0C, 1);
    sendData(0x0F, 0);
}

int main(void){
Init();
// setup I2C
wiringPiSPISetup(spi0,10000000);
while(1){
for (int i=0;i<=7;i++){

	FILE *tempfile;
        float temp;
        tempfile = fopen("/sys/class/thermal/thermal_zone0/temp","r");
        fscanf(tempfile, "%f",&temp);
        temp = temp / 1000;
        fclose(tempfile);
	delay(1);
	display(temp,i);


}
}
}


