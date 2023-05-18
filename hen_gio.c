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
int h,m,s,n=0;
char x[20];
char time_read[10];
char do_sang[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static unsigned long ontime, duration, inten, durainten;
int j=0,tang=1,nhap_nhay=0;
void sendData(uint8_t address, uint8_t data){
    buf[0] = address;
    buf[1] = data;
    wiringPiSPIDataRW(spi0, buf, 2);
}


void Init(){
    // set decode mode: 0x09
    sendData(0x09,0xDB);
    // set intensity: 0x0A09
    sendData(0x0A, 9);
    // scan limit: 0x0B07
    sendData(0x0B, 7);
    // no shutdown, turn off display test
    sendData(0x0C, 1);
    sendData(0x0F, 0);
}

void intensity(){
sendData(0x0A, do_sang[j]);
inten=millis();
if(j==0)
tang=1;
else if(j==15)
tang=0;
if(tang==1)
j++;
if(tang==0)
j--;

}

void display(int hour, int min, int32_t temp){
// set decode mode: 0x09
sendData(0x09,0xDB);
ontime=millis();
sendData(3,0x06);
sendData(6,0x01);
sendData(4,min%10);
sendData(5,min/10);
sendData(7,hour%10);
sendData(8,hour/10);
sendData(1,temp%10);
sendData(2,temp/10);
printf("\n%2d\n", temp);
}

int main(void){
Init();
// setup I2C
wiringPiSPISetup(spi0,10000000);
printf("nhap thoi gian dung he thong theo cu phap 'hh:mm:ss': \n");
fgets(x, sizeof(x), stdin);
puts(x);
for(int i=0; i<=strlen(x); i++){
if(isdigit(x[i])){
time_read[n] = x[i];
n++;
time_read[n] = '\0';
}
}
h =atoi(time_read);
s=h%100;
m=(h%10000)/100;
h=h/10000;
printf("blink at: %02d:%02d:%02d", h,m,s);
ontime=millis();
inten=millis();

while(1){
        FILE *tempfile;
        float temp;
	int hour, min, sec;
        tempfile = fopen("/sys/class/thermal/thermal_zone0/temp","r");
        fscanf(tempfile, "%f",&temp);
        temp = temp / 1000;
        fclose(tempfile);

        time_t rawtime;
        struct tm *ct;

        time (&rawtime);
        ct = localtime (&rawtime);
        // dd/mm/yyyy hh:mm:ss temp
        printf ("\r %02d/%02d/%04d %02d:%02d:%02d", ct->tm_mday,ct->tm_mon+1,ct->tm_year+1900,ct->tm_hour,ct->tm_min,ct->tm_sec);
	hour=ct->tm_hour;
	min=ct->tm_min;
	sec=ct->tm_sec;
	duration=millis()-ontime;
	durainten=millis()-inten;
	if(duration>=2000)
	display(hour,  min, temp);
        fflush(stdout);
	if(hour==h && min==m && sec>=s)
	nhap_nhay=1;
	if(nhap_nhay==1 && durainten>=50)
	intensity();

    }

    return 0;
}

