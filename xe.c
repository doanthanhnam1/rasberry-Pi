#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <time.h>  // time_t, struct tm, time, localtime 
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define Sample_rate         25
#define Config              26
#define Gygro_config        27
#define Acc_config          28
#define Interrupt           56
#define PWR_Managment       107
#define Acc_X               59
#define Acc_Y               61
#define Acc_Z               63
#define TemptHigh           65
#define TemptLow            66
#define spi0   0
char do_sang[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
uint8_t buf[2];
int mpu;
int j=0,tang=1;
void sendData(uint8_t address, uint8_t data){
    buf[0] = address;
    buf[1] = data;
    wiringPiSPIDataRW(spi0, buf, 2);
}

int read_sensor(unsigned char sensor){
    int16_t high, low, data;
    high = wiringPiI2CReadReg8(mpu, sensor );
    low = wiringPiI2CReadReg8(mpu, sensor  + 1 );
    data = (high<<8) | low;
    return data;
}

void Init_MPU6050(void){
    //resigter 25 -> 28 , 55, 56 ,107
    //sample rate 500hz
    wiringPiI2CWriteReg8(mpu, Sample_rate, 0x011);
    // no ext clock, DLFP 44hz
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x03);
    // gyro FS: +- 250 o/s
    wiringPiI2CWriteReg8(mpu, Gygro_config, 0x00);
    // acc FS: +-4g
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x08);
    // ennable interrupt Data ready
    wiringPiI2CWriteReg8(mpu, Interrupt,1);
    //nguon xung gyro X
    wiringPiI2CWriteReg8(mpu, PWR_Managment,0x01);
}

void Init(){
    // set decode mode: 0x09
    sendData(0x09,0x1C);
    // set intensity: 0x0A09
    sendData(0x0A, 9);
    // scan limit: 0x0B07
    sendData(0x0B, 7);
    // no shutdown, turn off display test
    sendData(0x0C, 1);
    sendData(0x0F, 0);
}

void blink(){
sendData(0x0A, do_sang[j]);
if(j==0)
tang=1;
else if(j==15)
tang=0;
if(tang==1)
j++;
if(tang==0)
j--;
printf("%d\n", j);
}

void display(float goc, float state){
sendData(0x09,0x1C);
int32_t integerPart1 =fabs(goc);
int32_t fractionalPart1 = (fabs(goc) - integerPart1) * pow(10,1);
int32_t number = integerPart1*pow(10,1) + fractionalPart1;
sendData(5,number/100);
sendData(4,(number/10)%10|0x80);
sendData(3,number%10);

if(state>0.2)
{
sendData(1,0x3D);
}

else
sendData(1,0x67);

if(goc>10.0)
{
sendData(8,0x3E);
sendData(7,0x67);
}

else if(goc<-10.0)
{
sendData(8,0x3D);
sendData(7,0x15);
}

else
{
sendData(8,0x01);
sendData(7,0x01);
}


sendData(6,0x01);
sendData(2,0x01);
}
int main(void){
mpu = wiringPiI2CSetup(0x68);
    //check I2C connenction
    if(wiringPiI2CReadReg8(mpu, 0x85) != 0x68){
        printf("connect fail!");
    }
Init();
Init_MPU6050();
// setup I2C
wiringPiSPISetup(spi0,10000000);
float v0=0,v;
while(1){
float  Ax = (float)read_sensor(Acc_X)/8192.0;
float  Ay = (float)read_sensor(Acc_Y)/8192.0;
float  Az = (float)read_sensor(Acc_Z)/8192.0;

float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;
if(Ax<=0.2 && Ax>=-0.2)
Ax=0;
v=v0+Ax*0.02;
v0=v;
printf("giatoc: %.2f  vantoc:%.2f\n",Ax,v);
display(roll,fabs(v));
if((v>11.1 || v<-11.1) && roll<-10.0)
blink();
delay(20);
}
}

