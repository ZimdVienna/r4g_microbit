/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

/*
 ABOUT
 Microbit program for the project Robo4girls (R4G)
 Educational micro:bit - robot plattform
 Project repository: https://github.com/ZimdVienna/Robo4girls
 Daniela Riedl, 2018
*/

#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "MusicalNotes.h"       //musical notes and songbook
#include "Pictures.h"

// MICROBIT OBJECTS
MicroBit uBit;                  // instance of the microbit class
MicroBitUARTService *uart;      // serial communication via Bluetooth Low Energy
MicroBitPin MelodyPin(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ANALOG);
//MicroBitPin SensorPinL(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ALL);
//MicroBitPin SensorPinR(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);

// comment out if you are not using waveshare motor board
MicroBitPin P8(MICROBIT_ID_IO_P8, MICROBIT_PIN_P8, PIN_CAPABILITY_ANALOG);
MicroBitPin P16(MICROBIT_ID_IO_P16, MICROBIT_PIN_P16, PIN_CAPABILITY_ANALOG);

//// comment in if you are using keyestudio- or elecfreak motor board
//MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ANALOG);
//MicroBitPin P2(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ANALOG);

//  MOTOR CONTROL VARIABLES
uint32_t velocity_1 = 1023;  // initial velocity motor1 (motor velocity from (1*64 - 1) = 63 to (16*64 - 1) = 1023
uint32_t velocity_2 = 1023;  // initial velocity motor2
const ManagedString forwards("v");  //name forwards to delineate from std::forward
const ManagedString backwards("z");
const ManagedString curveRight("r");
const ManagedString curveLeft("l");
const ManagedString turnRight("R"); //turns right around own middle axis
const ManagedString turnLeft("L");  //turns left around own middle axis
const ManagedString stop("s");

// FUNCTION DECLARATIONS
unsigned char set_direction(ManagedString direction, unsigned char *bitmasks);  // Set given direction via bitmask
void setMotorPins(ManagedString msg);    // Set pins of motor board, choose your board on the bottom of the file
void changeMotorVelocity(ManagedString msg);
void playMelody(ManagedString msg);
void turnDisplay(ManagedString msg);
void wait(ManagedString msg);
void showPictureOrText(ManagedString msg);
void moveBot(ManagedString msg);
void showSensorValue(ManagedString msg);

// CALLBACK FUNCTIONS
void onConnected(MicroBitEvent) {
    //>! Receive and handle commands, send "OK" back to app if executed successfully
    ManagedString msg = "R4G";
    ManagedString eom(":"); // Message delimiter
    while(1) {
        msg = uart->readUntil(eom);
        switch(msg.charAt(0)) {
            case 'B': {
                moveBot(msg);
                break;}
            case 'M': {
                playMelody(msg);
                break;}
            case 'G': {
                changeMotorVelocity(msg);
                break;}
            case 'S': {
                showSensorValue(msg);
                break;}
            case 'T': {
                turnDisplay(msg);
                break;}
            case 'W': {
                wait(msg);
                break;}
            case 'A': {
                showPictureOrText(msg);
                break;}
            default: {
                uBit.display.scroll(msg);
                break;}
        }
        // MelodyPin.setAnalogValue(0);
        // setMotorPins("s");  // stop motor
        uart->send("OK\n"); // send confirmation to app
    }
}

void onDisconnected(MicroBitEvent) {
    //>! If disconnected from bluetooth client scroll D
    uBit.display.scroll("D");
}

void onButtonB(MicroBitEvent) {
    //<! stops motors when ButtonB is clicked long
    setMotorPins("s");
    uBit.display.scroll("stop");
}

// MAIN
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.display.scrollAsync("R4G");
    uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);
    
    // Serial communication via uart
    uBit.serial.baud(9600);
    uBit.serial.send("A\r\n");
    
    // Define callback functions for events
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonB);
    
//    // Sensors
//    sensorRight.eventOn(MICROBIT_PIN_EVENT_ON_TOUCH));
//    sensorLeft.eventOn(MICROBIT_PIN_EVENT_ON_TOUCH));
//    uBit.messageBus.listen(MICROBIT_ID_IO_P1, MICROBIT_PIN_EVT_FALL, onTouch);
//    uBit.messageBus.listen(MICROBIT_ID_IO_P2, MICROBIT_PIN_EVT_FALL, onTouch)
    
    uart = new MicroBitUARTService(*uBit.ble,32,32);
    uBit.thermometer.setPeriod(2000);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

// FUNCTIONS
void showSensorValue(ManagedString msg) {
    switch (msg.charAt(1)) {
        case 't': {
            // uBit.thermometer.setCalibration(uBit.thermometer.getOffset());
            uBit.display.scroll(uBit.thermometer.getTemperature());
            break;}
        default:{
            uBit.display.scroll(msg);
            break;}
    }
}

void wait(ManagedString msg) {
    int wait_time = (uint32_t)((msg.charAt(1)-'0') * 1000);
    wait_time = wait_time + (uint32_t)((msg.charAt(3)-'0') * 100);
    if (wait_time >= 100 && wait_time <= 9900) {
        uBit.sleep(wait_time);
    }
    else {
        uBit.display.scroll(msg);
    }
}

void playMelody(ManagedString msg) {
    //>! play Song from songbook in musicalNotes.h
    if((msg.charAt(1)-'0') < 1 || (msg.charAt(1)-'0') > storedSongs){
        uBit.display.scroll(msg);
        return;
    }
    int songidx = (int)(msg.charAt(1)-'0') - 1;
    for(int i = 0; SONGS[songidx][i] != -1; i++){
        MelodyPin.setAnalogValue(511);  // set duty cycle
        MelodyPin.setAnalogPeriodUs((int)(1000000/SONGS[songidx][i]));
        uBit.sleep(BEATS[songidx][i]);
        MelodyPin.setAnalogValue(0);
        uBit.sleep(50);
    }
}

void turnDisplay(ManagedString msg) {
    int degrees = msg.charAt(1);
    if(degrees == '0') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);
    }
    else if(degrees == '9' && msg.charAt(2) == '0') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_90);
    }
    else if(degrees == '1' && msg.charAt(2) == '8') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_180);
    }
    else if(degrees == '2' && msg.charAt(2) == '7') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_270);
    }
    else {
        uBit.display.scroll(msg);
    }
}

void showPictureOrText(ManagedString msg) {
    //>! Show picture or scroll text according to msg from app
    int idx = msg.charAt(1) - '0';
    if(idx > storedPictures){
        uBit.display.scroll(msg);
        return;
    } else if(idx == 0) {
        uBit.display.clear();
        return;
    }
    int time_to_shine = (msg.charAt(2) - '0') * 1000;
    if(time_to_shine < 0 || time_to_shine > 9000){
        uBit.display.scroll(msg);
        return;
    }
    MicroBitImage i((ImageData*)PICTURES[idx-1]);
    if(time_to_shine == 0) {
        uBit.display.printAsync(i);
    } else {
        uBit.display.print(i,0,0,0,time_to_shine);
        uBit.display.clear();
    }
}

void moveBot(ManagedString msg) {
    uint32_t duration = 0;
    if((msg.charAt(2)-'0') < 0 || (msg.charAt(2)-'0') > 10){
        uBit.display.scroll(msg);
        return;
    }
    duration = (uint32_t)((msg.charAt(2)-'0') * 1000);
    duration = duration + (uint32_t)((msg.charAt(4)-'0') * 100);
    setMotorPins(msg);
    uBit.sleep(duration);
    setMotorPins("s");
}

void changeMotorVelocity(ManagedString msg) {
    char motor = msg.charAt(1);
    uint32_t val = (uint32_t)((msg.charAt(2)-'0') * 10 + (msg.charAt(3)-'0'));
    if(val < 1 || val > 31){
        uBit.display.scroll(msg);
        return;
    }
    int vel = int(val * 32 + 31);
    switch(motor){
        case '1': {
            velocity_1 = vel;
            break;
        }
        case '2': {
            velocity_2 = vel;
            break;
        }
        case 'b': {
            velocity_1 = vel;
            velocity_2 = vel;
            break;
        }
        default: {
            uBit.display.scroll(msg);
            break;
        }
    }
}

unsigned char set_direction(ManagedString direction, unsigned char *bitmasks){
    /**
     * Set a bitmask for given direction
     * Returns bitmask for direction
     */
    unsigned char moveMask = 0;
    if(direction == forwards)
        moveMask = bitmasks[0];
    else if(direction == backwards)
        moveMask = bitmasks[1];
    else if(direction == curveLeft)
        moveMask = bitmasks[2];
    else if(direction == curveRight)
        moveMask = bitmasks[3];
    else if(direction == turnLeft)
        moveMask = bitmasks[4];
    else if(direction == turnRight)
        moveMask = bitmasks[5];
    else
        moveMask = 0;   //stop
    return moveMask;
}

// Choose your board or write your own motor control
void setMotorPins(ManagedString msg) {
    /**
     * Waveshare Motor Driver Board for micro:bit
     * Motor A in1 = pin 13,    Motor A in2 = pin 12
     * Motor B in1 = pin 14,    Motor B in2 = pin 15
     * Motor A PWM = pin 8,     Motor B PWM = pin 16
     * bitmask for motor control: 0 0 P16 P15 P14 P8 P12 P13
     */
    unsigned char m1_pin1 = 1, m1_pin2 = 2, m1_pwm = 4, m2_pin1 = 8, m2_pin2 = 16, m2_pwm = 32; // bitmasks for motorPins
    unsigned char bitmasks[6] = {45,54,5,40,53,46}; // bitmasks for directions
    ManagedString direction(msg.charAt(1));
    unsigned char moveMask = 0;
    moveMask = set_direction(direction,bitmasks);
    //set the pin values according to direction
    uBit.io.P13.setDigitalValue(m1_pin1 & moveMask);                // dir motor1
    uBit.io.P12.setDigitalValue((m1_pin2 & moveMask)/m1_pin2);
    uBit.io.P14.setDigitalValue((m2_pin1 & moveMask)/m2_pin1);      // dir motor2
    uBit.io.P15.setDigitalValue((m2_pin2 & moveMask)/m2_pin2);
    P8.setAnalogValue(((m1_pwm & moveMask)/m1_pwm) * velocity_1);   // pwm motor1
    P16.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2);  // pwm motor2
}

//void setMotorPins(ManagedString msg) {
//    /**
//     * ElecFreaks Motor:bit Board
//     * Motor 1 PWM = P1,    Motor1 direction = P8 (LOW = CC, HIGH = C)
//     * Motor 2 PWM = P2,    Motor2 direction = P12(LOW = CC, HIGH = C)
//     * bitmask motor control: X X X X P12 P2 P8 P1
//     * bitmasks for motor pins:
//     */
//    unsigned char m1_pwm = 1, m1_dir = 2, m2_pwm = 4, m2_dir = 8;
//    unsigned char bitmasks[6] = {15,5,12,3,13,7};   // bitmasks for directions
//    ManagedString direction(msg.charAt(1));
//    unsigned char moveMask = set_direction(direction,bitmasks);
//    // Set the pin values according to direction
//    uBit.io.P12.setDigitalValue((m2_dir & moveMask)/m2_dir);              //dir motor2
//    uBit.io.P8.setDigitalValue((m1_dir & moveMask)/m1_dir);               //dir motor1
//    uBit.io.P1.setAnalogValue((m1_pwm & moveMask)* velocity_1);           //pwm motor1
//    uBit.io.P2.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2); //pwm motor2
//}

//void setMotorPins(ManagedString msg) {
//    /**
//     * Keyestudio Motor Driver Board v1.8
//     * Enable = pin14
//     * Motor1 CW = pin12,   Motor1 CCW = pin13
//     * Motor2 CW = pin15,   Motor2 CCW = pin16
//     * Motor1 PWM = pin1,   Motor2 PWM = pin2
//     * Bitmask motor control: X X X P15 P16 P12 P13 P14
//     */
//    unsigned char bitmasks[6] = {11,21,9,3,13,19};  // bitmasks for directions
//    ManagedString direction(msg.charAt(1));
//    unsigned char moveMask = set_direction(direction,bitmasks);
//    // set motor pins according to direction
//    uBit.io.P1.setAnalogValue(velocity_1);
//    uBit.io.P2.setAnalogValue(velocity_2);
//    uBit.io.P12.setDigitalValue((4 & moveMask)/4);
//    uBit.io.P13.setDigitalValue((2 & moveMask)/2);
//    uBit.io.P15.setDigitalValue((16 & moveMask)/16);
//    uBit.io.P16.setDigitalValue((8 & moveMask)/8);
//    uBit.io.P14.setDigitalValue(1 & moveMask);
//}

