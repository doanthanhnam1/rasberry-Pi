
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
uint8_t buf[2];
int mpu;

unsigned char dung[8]={0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
unsigned char bac1[8]={0x00,0x00,0x18,0x18,0x18,0x00,0x00,0x00};
unsigned char bac2[8]={0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x00};
unsigned char bac3[8]={0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00};
unsigned char dong1[8]={0x00,0x00,0x00,0x1c,0x1c,0x00,0x00,0x00};
unsigned char dong2[8]={0x00,0x00,0x00,0x1e,0x1e,0x00,0x00,0x00};
unsigned char dong3[8]={0x00,0x00,0x00,0x1f,0x1f,0x00,0x00,0x00};
unsigned char tay1[8]={0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00};
unsigned char tay2[8]={0x00,0x00,0x00,0x78,0x78,0x00,0x00,0x00};
unsigned char tay3[8]={0x00,0x00,0x00,0xf8,0xf8,0x00,0x00,0x00};
unsigned char nam1[8]={0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x00};
unsigned char nam2[8]={0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00};
unsigned char nam3[8]={0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18};
unsigned char dongbac1[8]={0x00,0x00,0x04,0x18,0x18,0x00,0x00,0x00};
unsigned char dongbac2[8]={0x00,0x02,0x04,0x18,0x18,0x00,0x00,0x00};
unsigned char dongbac3[8]={0x01,0x02,0x04,0x18,0x18,0x00,0x00,0x00};
unsigned char taybac1[8]={0x00,0x00,0x20,0x18,0x18,0x00,0x00,0x00};
unsigned char taybac2[8]={0x00,0x40,0x20,0x18,0x18,0x00,0x00,0x00};
unsigned char taybac3[8]={0x80,0x40,0x20,0x18,0x18,0x00,0x00,0x00};
unsigned char taynam1[8]={0x00,0x00,0x00,0x18,0x18,0x20,0x00,0x00};
unsigned char taynam2[8]={0x00,0x00,0x00,0x18,0x18,0x20,0x40,0x00};
unsigned char taynam3[8]={0x00,0x00,0x00,0x18,0x18,0x20,0x40,0x80};
unsigned char dongnam1[8]={0x00,0x00,0x00,0x18,0x18,0x04,0x00,0x00};
unsigned char dongnam2[8]={0x00,0x00,0x00,0x18,0x18,0x04,0x02,0x00};
unsigned char dongnam3[8]={0x00,0x00,0x00,0x18,0x18,0x04,0x02,0x01};




void sendData(uint8_t address, uint8_t data){
    buf[0] = address;
    buf[1] = data;
    wiringPiSPIDataRW(spi0, buf, 2);
}


void hien_thi(float x, float y){

if (x>5 && x<25){  //bac
for(int i=0;i<=7;i++){
sendData(i+1,bac1[7-i]);
}
}

if (x>=25 && x<45){
for(int i=0;i<=7;i++){
sendData(i+1,bac2[7-i]);
}
}

if (x>=45){
for(int i=0;i<=7;i++){
sendData(i+1,bac3[7-i]);
}
}

if (x<-5 && x>-25){  //nam
for(int i=0;i<=7;i++){
sendData(i+1,nam1[7-i]);
}
}

else if (x<=-25 && x>-45){
for(int i=0;i<=7;i++){
sendData(i+1,nam2[7-i]);
}
}

if (x<=-45){
for(int i=0;i<=7;i++){
sendData(i+1,nam3[7-i]);
}
}

if (y>5 && y<25){    //dong
for(int i=0;i<=7;i++){
sendData(i+1,dong1[7-i]);
}
}

if (y>=25 && y<45){
for(int i=0;i<=7;i++){
sendData(i+1,dong2[7-i]);
}
}

if (y>=45){
for(int i=0;i<=7;i++){
sendData(i+1,dong3[7-i]);
}
}

if (y<-5 && y>-25){  //tay
for(int i=0;i<=7;i++){
sendData(i+1,tay1[7-i]);
}
}

if (y<=-25 && y>-45){
for(int i=0;i<=7;i++){
sendData(i+1,tay2[7-i]);
}
}

if (y<=-45){
for(int i=0;i<=7;i++){
sendData(i+1,tay3[7-i]);
}
}

if ((x>5 && x<10 && y>5) || (y>5 && y<10 && x>5) ){  //dong bac
for(int i=0;i<=7;i++){
sendData(i+1,dongbac1[7-i]);
}
}

if ((x>=10 && x<20 && y>=10) || (y>=10 && y<20 && x>=10)){
for(int i=0;i<=7;i++){
sendData(i+1,dongbac2[7-i]);
}
}

if (x>=30 && y>=30){
for(int i=0;i<=7;i++){
sendData(i+1,dongbac3[7-i]);
}
}

if ((x>5 && x<10 && y<-5) || (y<-5 && y>-10 && x>5)){  //tay bac
for(int i=0;i<=7;i++){
sendData(i+1,taybac1[7-i]);
}
}

if ((x>=10 && x<20 && y<=-10) || (y<=-10 && y>-20 && x>=10)){
for(int i=0;i<=7;i++){
sendData(i+1,taybac2[7-i]);
}
}

if (x>=30 && y<=-30){
for(int i=0;i<=7;i++){
sendData(i+1,taybac3[7-i]);
}
}

if ((x<-5 && x>-10 && y>5) || (y>5 && y<10 && x<-5) ){  //dong nam
for(int i=0;i<=7;i++){
sendData(i+1,dongnam1[7-i]);
}
}

if ((x<=-10 && x>-20 && y>=10) || (y>10 && y<20 && x<=-10)){
for(int i=0;i<=7;i++){
sendData(i+1,dongnam2[7-i]);
}
}

if (x<=-30 && y>=30){
for(int i=0;i<=7;i++){
sendData(i+1,dongnam3[7-i]);
}
}

if ((x<-5 && x>-10 && y<-5) || (y<-5 && y>-10 && x<-5)){  //tay nam
for(int i=0;i<=7;i++){
sendData(i+1,taynam1[7-i]);
}
}

if ((x<=-10 && x>-20 && y<=-10) || (y<=-10 && y>-20 && x<=-10)){
for(int i=0;i<=7;i++){
sendData(i+1,taynam2[7-i]);
}
}

if (x<=-30 && y<=-30){
for(int i=0;i<=7;i++){
sendData(i+1,taynam3[7-i]);
}
}

else{      //dung yen
for(int i=0;i<=7;i++){
sendData(i+1,dung[7-i]);
}
}




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

void Init_MPU6050(void){
    //resigter 25 -> 28 
    //sample rate 500hz
    wiringPiI2CWriteReg8(mpu, Sample_rate, 0x01);
    // no ext clock, DLFP 44hz
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x03);
    // gyro FS: +- 500 o/s
    wiringPiI2CWriteReg8(mpu, Gygro_config, 0x08);
    // acc FS: +-4g
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x08);

}



int read_sensor(unsigned char sensor){
int16_t high, low, data;
    high = wiringPiI2CReadReg8(mpu, sensor );
    low = wiringPiI2CReadReg8(mpu, sensor  + 1 );
    data = (high<<8) | low;
    return data;
}


int main(void){
// setup I2C
wiringPiSPISetup(spi0,10000000);
mpu = wiringPiI2CSetup(0x68);
//check I2C connenction
if(wiringPiI2CReadReg8(mpu, 0x85) != 0x68)
printf("connect fail!");
// thiet lap che do do
Init_MPU6050();
// thiet lap che do led
init();
while (1){
float  Ax = (float)read_sensor(Acc_X)/8192.0;
float  Ay = (float)read_sensor(Acc_Y)/8192.0;
float  Az = (float)read_sensor(Acc_Z)/8192.0;

float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;    
float pitch = atan2(Ax, sqrt(pow(Ay, 2)+pow(Az, 2)))*180/M_PI;
hien_thi(roll, pitch);
}
}
