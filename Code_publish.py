#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64
import RPi.GPIO as GPIO
import math
import smbus
import spidev


SPI_SPEED_HZ = 10000000
SPI_DEVICE = 0
SPI_CS_PIN = 8

DUNG = [0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00]
LEN = [0x08, 0x0c, 0xfe, 0xff, 0xff, 0xfe, 0x0c, 0x08]
XUONG = [0x10, 0x30, 0x7f, 0xff, 0xff, 0x7f, 0x30, 0x10]
PHAI = [0x18, 0x3c, 0x7e, 0xff, 0x3c, 0x3c,0x3c, 0x3c]
TRAI = [0x3c, 0x3c, 0x3c, 0x3c, 0xff,0x7e, 0x3c, 0x18]
sensor0 = 16 
sensor1 = 20
sensor2 = 21
led0 = 17    
led1 = 27 
led2=25
UP= 22
DOWN = 5
LEFT = 6
RIGHT = 13
increase = 19
decrease = 26
msg_left = Float64()
msg_right = Float64()
acc1=0
type = 0
i=1.0
type0=0

MPU6050_ADDR = 0x68
MPU6050_ACCEL_XOUT_H = 0x3B
MPU6050_ACCEL_YOUT_H = 0x3D
MPU6050_ACCEL_ZOUT_H = 0x3F
MPU6050_GYRO_XOUT_H = 0x43
MPU6050_GYRO_YOUT_H = 0x45
MPU6050_GYRO_ZOUT_H = 0x47
MPU6050_TEMP_OUT_H = 0x41

spi = spidev.SpiDev()
spi.open(SPI_DEVICE, 0)
spi.max_speed_hz = SPI_SPEED_HZ
spi.mode = 0b00

bus = smbus.SMBus(1)



def sensor_callback(channel):
    global acc1, type
    msg_left = Float64()
    msg_right = Float64()
    if GPIO.input(channel) and channel == sensor0:
        GPIO.output(led0, GPIO.HIGH)
        type=1
    if not GPIO.input(channel) and channel == sensor0:
        GPIO.output(led0, GPIO.LOW)
        type=0
        msg_left.data = 0.0
        msg_right.data = 0.0
        for j in range(8):
            send_data(j + 1, DUNG[7 - j])
        left_publisher.publish(msg_left)
        right_publisher.publish(msg_right)
    if GPIO.input(channel) and channel == sensor1 and type==1:
        acc1=1
        GPIO.output(led1, GPIO.HIGH)
        GPIO.output(led2, GPIO.LOW)
    else:
        acc1=0
        GPIO.output(led1, GPIO.LOW)
    if GPIO.input(channel) and channel == sensor2 and type==1 and acc1==0:
        msg_left = Float64()
        msg_right = Float64()
        GPIO.output(led2, GPIO.HIGH)
        while GPIO.input(channel) and channel==sensor2 and GPIO.input(sensor0) and acc1==0 and type==1:
            if GPIO.input(channel) and channel==sensor1 and type==1:
                acc1=1
            if not GPIO.input(channel) and channel==sensor0:
                type=0
            GPIO.output(led1, GPIO.LOW)
            GPIO.output(led2, GPIO.HIGH)

            accel_x = read_word_2c(MPU6050_ACCEL_XOUT_H)
            accel_y = read_word_2c(MPU6050_ACCEL_YOUT_H)
            accel_z = read_word_2c(MPU6050_ACCEL_ZOUT_H)

            # Calculate angle
            angle_x = math.atan2(accel_y, math.sqrt(accel_x**2 + accel_z**2))
            angle_y = math.atan2(accel_x, math.sqrt(accel_y**2 + accel_z**2))

            # Convert angle_x to degrees and publish data
            angle_x_deg = math.degrees(angle_x)
            angle_y_deg = math.degrees(angle_y)

            if angle_y_deg>=15.0:
                msg_left.data = -2.0*i
                msg_right.data = 2.0*i
                rospy.loginfo("OK15y")
                for j in range(8):
                    send_data(j + 1, XUONG[7 - j])
            elif angle_y_deg<=-15.0:
                rospy.loginfo("OK-15y")
                msg_left.data = 2.0*i
                msg_right.data = -2.0*i
                for j in range(8):
                    send_data(j + 1, LEN[7 - j])
            elif angle_x_deg<=-15.0:
                rospy.loginfo("OK-15x")
                msg_left.data = 1.0*i
                msg_right.data =-3.0*i
                for j in range(8):
                    send_data(j + 1, TRAI[7 - j])
            elif angle_x_deg>=15.0:
                rospy.loginfo("OK15x")
                msg_left.data = 3.0*i
                msg_right.data =-1.0*i
                for j in range(8):
                    send_data(j + 1, PHAI[7 - j])
            else:
                msg_left.data = 0.0
                msg_right.data = 0.0
                rospy.loginfo("OK")
                for j in range(8):
                    send_data(j + 1, DUNG[7 - j])
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
        msg_left.data = 0.0
        msg_right.data = 0.0
        for j in range(8):
            send_data(j + 1, DUNG[7 - j])
        left_publisher.publish(msg_left)
        right_publisher.publish(msg_right)
        GPIO.output(led2, GPIO.LOW)






def read_word_2c(addr):

    high = bus.read_byte_data(MPU6050_ADDR, addr)
    low = bus.read_byte_data(MPU6050_ADDR, addr+1)
    value = ((high << 8) | low)
    if (value > 32768):
        value = value - 65536
    return value






def button_callback_up(channel):
    global acc1, i
    msg_left = Float64()
    msg_right = Float64()

    if acc1 == 1:
        if GPIO.input(channel):
            msg_left.data = 2.0*i
            msg_right.data = -2.0*i
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button UP")
            for j in range(8):
                    send_data(j + 1, LEN[7 - j])
        else:
            msg_left.data = 0.0
            msg_right.data = 0.0
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button")
            for j in range(8):
                    send_data(j + 1, DUNG[7 - j])


def button_callback_down(channel):
    global acc1, i
    msg_left = Float64()
    msg_right = Float64()
    if acc1 == 1:
        if GPIO.input(channel):
            msg_left.data = -2.0*i
            msg_right.data = 2.0*i
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button DOWN")
            for j in range(8):
                    send_data(j + 1, XUONG[7 - j])
        else:
            msg_left.data = 0.0
            msg_right.data = 0.0
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button")
            for j in range(8):
                    send_data(j + 1, DUNG[7 - j])



def button_callback_left(channel):
    global acc1, i
    msg_left = Float64()
    msg_right = Float64()
    if acc1 == 1:
        if GPIO.input(channel):
            msg_left.data = 1.0*i
            msg_right.data =-3.0*i
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button LEFT")
            for j in range(8):
                send_data(j + 1, TRAI[7 - j])
        else: 
            msg_left.data = 0.0
            msg_right.data = 0.0
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button")
            for j in range(8):
                    send_data(j + 1, DUNG[7 - j])




def button_callback_right(channel):
    global acc1, i
    msg_left = Float64()
    msg_right = Float64()
    if acc1 == 1:
        if GPIO.input(channel):
            msg_left.data = 3.0*i
            msg_right.data = -1.0*i 
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button RIGHT")
            for j in range(8):
                    send_data(j + 1, PHAI[7 - j])
        else:
            msg_left.data = 0.0
            msg_right.data = 0.0
            left_publisher.publish(msg_left)
            right_publisher.publish(msg_right)
            rospy.loginfo("Button")
            for j in range(8):
                    send_data(j + 1, DUNG[7 - j])




def vel_callback(channel):
    global type, i

    if channel == increase and type==1:
        i+=1.0
        if i>=10.0:
            i=10.0
    elif channel == decrease and type==1:
        i-=1.0
        if i<=1.0:
            i=1.0



def send_data(address, data):
    spi.xfer2([address, data])

send_data(0x09, 0x00)
send_data(0x0A, 0x09)
send_data(0x0A, 0x01)
send_data(0x0B, 0x07)
send_data(0x0C, 0x01)
send_data(0x0F, 0x00)

    


GPIO.setmode(GPIO.BCM)
GPIO.setup(sensor0, GPIO.IN)
GPIO.setup(led0, GPIO.OUT)
GPIO.add_event_detect(sensor0, GPIO.BOTH, callback=sensor_callback)
GPIO.setup(sensor1, GPIO.IN)
GPIO.add_event_detect(sensor1, GPIO.BOTH, callback=sensor_callback)
GPIO.setup(sensor2, GPIO.IN)
GPIO.add_event_detect(sensor2, GPIO.BOTH, callback=sensor_callback)
GPIO.setup(led1, GPIO.OUT)
GPIO.setup(led2, GPIO.OUT)
GPIO.setup(UP, GPIO.IN)
GPIO.setup(DOWN, GPIO.IN)
GPIO.setup(LEFT, GPIO.IN)
GPIO.setup(RIGHT, GPIO.IN)
GPIO.add_event_detect(UP, GPIO.BOTH, callback=button_callback_up) 
GPIO.add_event_detect(DOWN, GPIO.BOTH, callback=button_callback_down)
GPIO.add_event_detect(LEFT, GPIO.BOTH, callback=button_callback_left)
GPIO.add_event_detect(RIGHT, GPIO.BOTH, callback=button_callback_right)
GPIO.setup(increase, GPIO.IN)
GPIO.setup(decrease, GPIO.IN)
GPIO.add_event_detect(increase, GPIO.BOTH, callback=vel_callback)
GPIO.add_event_detect(decrease, GPIO.BOTH, callback=vel_callback)

bus.write_byte_data(MPU6050_ADDR, 0x19, 0x01)
bus.write_byte_data(MPU6050_ADDR, 0x1C, 0x03)
bus.write_byte_data(MPU6050_ADDR, 0x1B, 0x08)
bus.write_byte_data(MPU6050_ADDR, 0x1C, 0x08)

rospy.init_node('publish')
left_publisher = rospy.Publisher('/my_diffbot/joint1_Velocity_controller/command', Float64, queue_size=10)
right_publisher = rospy.Publisher('/my_diffbot/joint2_Velocity_controller/command', Float64, queue_size=10)

rate = rospy.Rate(50)


while not rospy.is_shutdown():
    rate.sleep()