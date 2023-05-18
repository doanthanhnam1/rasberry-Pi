#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<wiringPi.h>
#include<wiringPiI2C.h> //gcc......-lwiringPi -lm
#include <wiringPiSPI.h>

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
#define spi0 0         

int mpu;
unsigned char Lx[8]={0x10,0x30,0x7f,0xff,0xff,0x7f,0x30,0x10};
unsigned char buf[2];

void sendData(unsigned char address, unsigned char data ){
buf[0] = address;
buf[1] = data;
wiringPiSPIDataRW(spi0,buf,2);
}


int Read_sesor(unsigned char Tempt_register){
    int16_t MSB, LSB, data;
    MSB = wiringPiI2CReadReg8(mpu, Tempt_register);
    LSB = wiringPiI2CReadReg8(mpu, Tempt_register + 1 );
    data = (MSB<<8) | LSB;
    return data;
}




    
void Init_MPU6050(void){
    //resigter 25 -> 28 , 55, 56 ,107
    //sample rate 100hz
    wiringPiI2CWriteReg8(mpu, Sample_rate, 0x09);
    // no ext clock, DLFP 94hz
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x02);
    // gyro FS: +- 500 o/s  
    wiringPiI2CWriteReg8(mpu, Gygro_config, 0x08);
    // acc FS: +-8g
    wiringPiI2CWriteReg8(mpu, Acc_config, 0x10);
    // ennable interrupt Data ready
    wiringPiI2CWriteReg8(mpu, Interrupt, 0x01);
    // chọn nguồn xung Gyro X
    wiringPiI2CWriteReg8(mpu, PWR_Managment,0x01);

}


int main(void){
    // setup I2C 
    wiringPiSPISetup(spi0,10000000);
    mpu = wiringPiI2CSetup(0x68);
    //check I2C connenction
    if(wiringPiI2CReadReg8(mpu, 0x85) != 0x68){
        printf("connect fail!");
    }
    // thiet lap che do do
    Init_MPU6050();

    while (1){
    // doc gia tri do
    float Ax = (float)Read_sesor(Acc_X) / 4096.0;
    float Ay = (float)Read_sesor(Acc_Y) / 4096.0;
    float Az = (float)Read_sesor(Acc_Z) / 4096.0;

    // Tính toán
    // float pitch = atan2(Ax, sqrt(pow(Ay, 2)+pow(Az, 2)))*180/M_PI; 
    // float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;
    float pitch = atan2(Ax, sqrt(pow(Ay, 2)+pow(Az, 2)))*180/M_PI; 

    float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;
        if(roll < 0){
            for(int i=0;i<=7;i++){
            sendData(i+1,Lx[7-i]);
        }}
        

    
    printf("goc nghieng truc X la: %f\n", roll);
    printf("goc nghieng truc Y la: %f\n", pitch);

     // doc gia tri do
    float data = (float)Read_sesor(TemptHigh)/340.0;
   
    // tinh toan
    float tempt_out = data + 36.53;
    
    printf("nhiet do hien tai cua cam bien la: %f\n", tempt_out);

    delay(2000);
    }
    return 0;

}
