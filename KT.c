/* Bai KT1
Doan Thanh Nam - 20146506
Vu_Duc_Binh - 20146478
Nguyen_Van_Tan - 20146527
*/


#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <softPwm.h>
#include <unistd.h>
#include <stdlib.h>

uint8_t led[3] = {3,2,0};  //khai bao chan GPIO cho led
#define BT1 24             //khai bao chan GPIO cho nut nhan
#define BT2 23
#define BT3 22
#define BT4 21
static unsigned long time, duration;  //bien khoang thoi gian
uint8_t r[7]={100,0,0,100,100,0,100}; //7 che do mau khac nhau
uint8_t g[7]={0,100,0,100,0,100,100};
uint8_t b[7]={0,0,100,0,100,100,100};
int blink0; // nhap nhay tan so xHz
int pos=0;  //giu trang thai nhap nhay tai mau thu j


void ngat()
{

if(digitalRead(BT1)==1)  //su kien nhan nut 1
{

}


if(digitalRead(BT2)==1)  //su kien nhan nut 2
{

}


if(digitalRead(BT3)==1)  //su kien nhan nut 3
{

}


if(digitalRead(BT4)==1)  //su kien nhan nut 4
{

}


}

void nhap_nhay()


int main(void)
{

wiringPiSetup(); //setup
for (int i=0; i<3; i++)
{
pinMode(led[i],OUTPUT);
softPwmCreate(led[i],0,100);
}

pinMode(BT1, INPUT);
pinMode(BT2, INPUT);
pinMode(BT3, INPUT);
pinMode(BT4, INPUT);
wiringPiISR(BT1, INT_EDGE_BOTH, &ngat); //su kien ngat khi nhan nut
wiringPiISR(BT2, INT_EDGE_BOTH, &ngat);
wiringPiISR(BT3, INT_EDGE_BOTH, &ngat);
wiringPiISR(BT4, INT_EDGE_BOTH, &ngat);

while(1) //chuong trinh chinh
{

}

}
