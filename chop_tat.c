#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPiI2C.h>

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
uint8_t mang1[3];
uint8_t mang2[3];

int Read_sesor(unsigned char Tempt_register){
    int16_t MSB, LSB, data;
    MSB = wiringPiI2CReadReg8(mpu, Tempt_register);
    LSB = wiringPiI2CReadReg8(mpu, Tempt_register + 1 );
    data = (MSB<<8) | LSB;
    return data;
}

void sendData(uint8_t address, uint8_t data){
    buf[0] = address;
    buf[1] = data;
    wiringPiSPIDataRW(spi0, buf, 2);
}

void Init(void){
    // set decode mode: 0x09
    sendData(0x09,0xEE);
    // set intensity: 0x0A09
    sendData(0x0A, 9);
    // scan limit: 0x0B07
    sendData(0x0B, 7);
    // no shutdown, turn off display test
    sendData(0x0C, 1);
    sendData(0x0F, 0);
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


void display_float(float num1, float num2){
    int32_t integerPart1 = num1;
    int32_t fractionalPart1 = (num1 - integerPart1) * pow(10,1);
    int32_t number1 = integerPart1*pow(10,1) + fractionalPart1;
    int32_t integerPart2 = num2;
    int32_t fractionalPart2 = (num2 - integerPart2) * pow(10,1);
    int32_t number2 = integerPart2*pow(10,1) + fractionalPart2;
    int dem=0;
while(dem<=5){

    // set scanlimit
    //sendData(0x0B, count-1);
    // dislay number
    for(int i=2; i<=4;i++){
        if(i==3){
if(dem==0)
	    mang1[i-2]=number1%10;
            sendData(i,(mang1[i-2])|0x80); // turn on dot segment
if(dem==0)
	    mang2[i-2]=number2%10;
            sendData(i+4,(mang2[i-2])|0x80); // turn on dot segment 
}
        else{
if(dem==0)
            mang1[i-2]=number1%10;
            sendData(i,mang1[i-2]);
if(dem==0)
            mang2[i-2]=number2%10;
            sendData(i+4,mang2[i-2]);
}
        number1 = number1/10;
	number2 = number2/10;
    // dislay o
}
    sendData(1,0x63);
    sendData(5,0x63);

delay(200);
dem+=1;

if(num1>45.0 || num2>45.0){
if(num1>45){
sendData(1,0x00);
for(int i=2;i<=4;i++)
sendData(i,0x0f);
}
if(num2>45){
sendData(5,0x00);
for(int i=6;i<=8;i++)
sendData(i,0x0f);
}
delay(200);
dem+=1;
}

}
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
// float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;
    float pitch = atan2(Ax, sqrt(pow(Ay, 2)+pow(Az, 2)))*180/M_PI;

    float roll = atan2(Ay, sqrt(pow(Ax, 2)+pow(Az, 2)))*180/M_PI;
    printf("goc nghieng truc X la: %f\n", roll);
    printf("goc nghieng truc Y la: %f\n", pitch);

    // set operational mode for max7219
    Init();
    display_float(fabs(pitch),fabs(roll));
}
}

