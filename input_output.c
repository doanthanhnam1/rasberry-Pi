/* Bai KT1
Doan Thanh Nam - 20146506 (CT5)
Vu_Duc_Binh - 20146478    (CT5)
Nguyen_Van_Tan - 20146527 (CT5)
*/


#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <softPwm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

uint8_t led[3] = {3,2,0};  //khai bao chan GPIO cho led
static unsigned long time, duration;  //bien khoang thoi gian
uint8_t r[8]={0,100,0,0,100,100,0,100}; //7 che do mau khac nhau
uint8_t g[8]={0,0,100,0,100,0,100,100};
uint8_t b[8]={0,0,0,100,0,100,100,100};
int N,reset=0; //random; dao trang thai <,>80
int ds=0, r=0, g=0, b=0; // 4 gia tri nhap 
int blink0, blink1,blink2;  // tang giam do sang 
int pos=0;  //giu trang thai nhap nhay tai mau thu i

void thucthi()//tao so random
{
N =20+rand()% 81;
duration=0;
}

void doimau()
{
time = millis();
for(int i=pos;i<=8;i++)
{
duration = millis()-time;
if (duration>1000) //1 giay sau se random lai so N
break;
if(i>7)
i=0;
softPwmWrite(led[0],r[i]);
softPwmWrite(led[1],g[i]);
softPwmWrite(led[2],b[i]);
delay(500);
pos=i;
}
N =20+rand()% 81;
duration=0;
}

void do_sang()
{
printf("nhap do sang, gia tri r g b: ");
scanf("%d %d %d %d",&ds, &r, &g, &b);
wiringPiSetup();
for (int i=0; i<3; i++)
{
pinMode(led[i], OUTPUT);
}
softPwmCreate(led[0],0,r);
softPwmCreate(led[1],0,g);
softPwmCreate(led[2],0,b);
}

void nhapnhay()
{
blink0=(r[pos])/12;
blink1=(g[pos])/12;
blink2=(b[pos])/12;
printf("Warnning\n");
printf("nhap ki tu c de reset\n");
time = millis();
for(int k=0;k<12;k++)     //sang dan  
{
duration = millis()-time;
if(kbhit() && duration <= 3000)
{
            char c[] = getch(); //Lấy ký tự từ bộ nhớ đệm và gắn vào biến c
            char b[]= "c";
            res = strcmp(c,b); //bien so sanh
            if(res == 0) //neu phim nhan la bien c
            break;
}
else if (duration > 3000)
{
reset= 1 //reset ve lai cau a
duration=0;
ds=0;  
}


softPwmWrite(led[0],k*blink0);
softPwmWrite(led[1],k*blink1);
softPwmWrite(led[2],k*blink2);
delay(14);
}

for(int k=11;k>=0;k--)     //toi dan  
{
duration = millis()-time;
if(kbhit() && duration <= 3000)
{
            char c[] = getch(); //Lấy ký tự từ bộ nhớ đệm và gắn vào biến c
            char b[]= "c";
            res = strcmp(c,b); //bien so sanh
            if(res == 0) //neu phim nhan la bien c
            break;
}
else if (duration > 3000)
{
reset= 1 //reset ve lai cau a
duration=0;
ds=0;  
}
if (duration > 3000 || reset==1)
break;
softPwmWrite(led[0],blink0*k);
softPwmWrite(led[1],blink1*k);
softPwmWrite(led[2],blink2*k);
delay(14);
}

}

void hien_thi()
{
if (r==0)
digitalWrite(led[0],LOW);
else
softPwmWrite(led[0],(r*ds)/100);


if (g==0)
digitalWrite(led[1],LOW);
else
softPwmWrite(led[1],(g*ds)/100);


if (b==0)
digitalWrite(led[2],LOW);
else
softPwmWrite(led[2],(b*ds)/100);

}

int main(void)
{

wiringPiSetup(); //setup
for (int i=0; i<3; i++)
{
pinMode(led[i],OUTPUT);
softPwmCreate(led[i],0,100);
}


while(1) //chuong trinh chinh
{
while (ds==0) //set gia tri mot lan duy nhat (cau a)
do_sang();
hien_thi();
r[0]=r*ds;
g[0]=g*ds;
b[0]=b*ds;
delay(1000);
reset=0;
while(reset==0)  //truong hop N <=80 hoac N>80
thucthi();        //tao so random cho cau b
{
if (N<=80)
doimau();
else
nhapnhay();
}
}

}
