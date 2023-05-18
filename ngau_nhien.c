#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(void)
{
while(1)
{
int r;
int b=0;
float c;
for (int i =0; i<10; i++)
	{
	b=b+rand()%101;
	}
c=b/10.0;
printf("trung binh cong la: %.2f \r", c);
fflush(stdout);
sleep(1);
}
return 0;
}
