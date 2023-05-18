#include <wiringPi.h>
#include <stdio.h>

// Define GPIO pins
#define DATA_PIN 0 // GPIO 17
#define CLK_PIN 1 // GPIO 18
#define CS_PIN 2 // GPIO 27

// Define LED matrix digit patterns
unsigned char digit_patterns[10][8] = {
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}, // 0
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, // 1
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}, // 2
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}, // 3
    {0x18, 0x3C, 0x3C, 0x66, 0x66, 0xFF, 0xFF, 0x66}, // 4
    {0xFC, 0xFC, 0xC0, 0xFC, 0xFE, 0x07, 0x07, 0xFE}, // 5
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}, // 6
    {0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3}, // 7
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}, // 8
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C}  // 9
};

void send_data(unsigned char data)
{
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(CLK_PIN, LOW);
        digitalWrite(DATA_PIN, data & 0x80);
        data <<= 1;
        digitalWrite(CLK_PIN, HIGH);
    }
}

void send_command(unsigned char command)
{
    digitalWrite(CS_PIN, LOW);
    send_data(command);
    digitalWrite(CS_PIN, HIGH);
}

void update_display(unsigned char *data)
{
    for(int row = 0; row < 8; row++)
    {
        send_command(0xB8 | row); // select row
        send_command(0x40); // select column 0

        for(int col = 0; col < 8; col++)
        {
            send_data(data[row*8]);
        }
    }
}
int main()
{
// Initialize wiringPi library
wiringPiSetup();

//Set GPIO pins as outputs
pinMode(DATA_PIN, OUTPUT);
pinMode(CLK_PIN, OUTPUT);
pinMode(CS_PIN, OUTPUT);

// Initialize LED matrix
send_command(0x0C); // turn on display
send_command(0x01); // clear display

// Display numbers 0-9
for(int i = 0; i < 1000; i++)
{
    update_display(digit_patterns[i]);
    delay(1000);
}

return 0;
}
    

