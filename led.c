#include <stdio.h>
#include <wiringPi.h>

#define BT1  24
#define BT2  25
#define LED  27

int main(void)
{
	//khoi tao wiringPi
	wiringPiSetup();
	//khai bao IO
	pinmode(BT1,INPUT);
	pinmode(BT2,INPUT);
	pinmode(LED,OUTPUT);
	int i=0;
	//loop
	while(1)
		{
		if (digitalread(BT1)==1)
			{
			i+=1;
			if (digitalread(BT2)==0 && (i%2==0))
			digitalwrite(LED,HIGH);
			else
				{
				digitalwrite(LED,HIGH);
				sleep(1);
				digitalwrite(LED,LOW);
				}
			}
		}
}
