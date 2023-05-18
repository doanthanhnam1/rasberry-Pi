#include <stdio.h>
#include <unistd.h>
#include <time.h>  // time_t, struct tm, time, localtime 
int stop=1;
int main(void){

    while(stop){
        FILE *tempfile;
        float temp;
	int p=30;
        tempfile = fopen("/sys/class/thermal/thermal_zone0/temp","r");
        fscanf(tempfile, "%f",&temp);
        temp = temp / 1000;
        fclose(tempfile);

        time_t rawtime;
        struct tm *ct;

        time (&rawtime);
        ct = localtime (&rawtime);
        // dd/mm/yyyy hh:mm:ss temp
        printf ("\r %02d/%02d/%04d %02d:%02d:%02d %.2f", ct->tm_mday,ct->tm_mon+1,ct->tm_year+1900,ct->tm_hour,ct->tm_min,ct->tm_sec, temp);
	if(p-ct->tm_min==0)
{
stop=0;
}
        fflush(stdout);
        sleep(1);
    }

    return 0;
}

