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

// Robo4girls R4G: educational micro:bit - robot plattform
// Project repository: https://github.com/ZimdVienna/Robo4girls

#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "MusicalNotes.h"

MicroBit uBit;                  // instance of the microbit class
MicroBitUARTService *uart;      // serial communication via Bluetooth Low Energy

// used microbit analog pins
MicroBitPin MelodyPin(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ANALOG);
MicroBitPin SensorPinL(MICROBIT_ID_IO_P10, MICROBIT_PIN_P10, PIN_CAPABILITY_ALL);
MicroBitPin SensorPinR(MICROBIT_ID_IO_P4, MICROBIT_PIN_P4, PIN_CAPABILITY_ALL);

// use of constants gets stored in flash memory (saves RAM)
// Melodies as integer arrays: 0 ends the song!
const int tusch[]       =   {NOTE_C4,NOTE_G3,NOTE_G3,NOTE_A3,NOTE_G3,NOTE_B3,NOTE_C4,0};
const int song2[]       =   {NOTE_C5,NOTE_B4,NOTE_G4,NOTE_C5,NOTE_B4,NOTE_E4,NOTE_C5,NOTE_C4,NOTE_G4,NOTE_A4,NOTE_C5,0};
const int starWars[]    =   {NOTE_G3,NOTE_G3,NOTE_G3,NOTE_C3,NOTE_G3,NOTE_F3,NOTE_E3,NOTE_D3,NOTE_C4,NOTE_G3,NOTE_F3,NOTE_E3,NOTE_D3,NOTE_C4,NOTE_G3,NOTE_F3,NOTE_E3,NOTE_F3,NOTE_D3,0};
const int superMario[]  =   {NOTE_E4,NOTE_E4,NOTE_E4,NOTE_C4,NOTE_E4,NOTE_G4,NOTE_G3,0};

// Delays to hold the notes (eg. 1/8, 1/4, 1/16 notes)
const int tusch_b[]     =   {250, 160, 160, 250,400,250,250,0};
const int beat2[]       =   {160, 160, 160, 800, 800, 160, 160, 160, 160, 800, 800,0};
const int starWars_b[]  =   {160, 160, 160,800,800,160,160,160,800,400,160,160,160,800,400,160,160,160,800,0};
const int superMario_b[]=   {160, 160, 250, 160,160,400,250,0};

// Songbook
const int *SONGS[] = {tusch,song2,starWars,superMario};
const int *BEATS[] = {tusch_b,beat2,starWars_b,superMario_b};
const int storedSongs = 4;

// Pictures: microbit images get normally stored in sram, to store them in flash:
const uint8_t smallHeart[] __attribute__ ((aligned (4)))= { 0xff, 0xff, 5, 0, 5, 0, 0,0,0,0,0, 0,1,0,1,0, 0,1,1,1,0, 0,0,1,0,0, 0,0,0,0,0 };
const uint8_t heart[] __attribute__ ((aligned (4)))     = { 0xff, 0xff, 5, 0, 5, 0, 0,1,0,1,0, 1,1,1,1,1, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0 };
const uint8_t happy[] __attribute__((aligned (4)))      = { 0xff, 0xff, 5, 0, 5, 0, 0,1,0,1,0, 0,1,0,1,0, 0,0,0,0,0, 1,0,0,0,1, 0,1,1,1,0 };
const uint8_t sad[] __attribute__((aligned (4)))        = { 0xff, 0xff, 5, 0, 5, 0, 0,1,0,1,0, 0,1,0,1,0, 0,0,0,0,0, 0,1,1,1,0, 1,0,0,0,1 };
const uint8_t angry[] __attribute__((aligned (4)))      = { 0xff, 0xff, 5, 0, 5, 0, 1,0,0,0,1, 0,1,0,1,0, 0,0,0,0,0, 1,1,1,1,1, 1,0,1,0,1 };
const uint8_t asleep[] __attribute__((aligned(4)))      = { 0xff, 0xff, 5, 0, 5, 0, 0,0,0,0,0, 1,1,0,1,1, 0,0,0,0,0, 0,1,1,1,0, 0,0,0,0,0 };
const uint8_t surprised[] __attribute__((aligned(4)))   = { 0xff, 0xff, 5, 0, 5, 0, 0,1,0,1,0, 0,0,0,0,0, 0,0,1,0,0, 0,1,0,1,0, 0,0,1,0,0 };
const uint8_t yes[] __attribute__((aligned(4)))         = { 0xff, 0xff, 5, 0, 5, 0, 0,0,0,0,0, 0,0,0,0,1, 0,0,0,1,0, 1,0,1,0,0, 0,1,0,0,0 };
const uint8_t no[] __attribute__((aligned(4)))          = { 0xff, 0xff, 5, 0, 5, 0, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,1,0,1,0, 1,0,0,0,1 };
// Picturebook
const uint8_t *PICTURES[] = {happy, heart, smallHeart, sad, angry, asleep, surprised, yes, no};
const int storedPictures = 9;
// load image from flash and show it:
// MicroBitImage i((ImageData*)heart);
// uBit.display.print(MicroBitImage, int x, int y, int alpha , int delay);

// Motor Control
uint32_t velocity_1 = 1023;  // initial velocity motor1 (motor velocity from (1*64 - 1) = 63 to (16*64 - 1) = 1023
uint32_t velocity_2 = 1023;  // initial velocity motor2
// constant variables for directions
const ManagedString forwards("v");      //name forwards to delineate from std::forward
const ManagedString backwards("z");
const ManagedString curveRight("r");
const ManagedString curveLeft("l");
const ManagedString turnRight("R");     //turns right around own middle axis
const ManagedString turnLeft("L");      //turns left around own middle axis
const ManagedString stop("s");

// set given direction via bitmasks for different motor control boards
unsigned char set_direction(ManagedString direction, unsigned char *bitmasks){
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
/* function to set pins of motor board, choose your board on the bottom of the file or write your own motor control */
void moveBot(ManagedString msg);

/******************** Functions ***************************/
    
void playMelody(int songidx) {
    //<! plays melody out of songbook
    for(int i = 0; SONGS[songidx][i] != 0; i++){
            MelodyPin.setAnalogValue(511);//set duty cycle
            MelodyPin.setAnalogPeriodUs((int)(1000000/SONGS[songidx][i]));
            uBit.sleep(BEATS[songidx][i]);
            MelodyPin.setAnalogValue(0);
            uBit.sleep(50);   
    }
}

void onConnected(MicroBitEvent) {
    //>! receives and handles commands, sends "OK" to app if successfull
    ManagedString msg = "R4G";
    ManagedString eom(":");
    uBit.display.scroll("C");
    
    while(1) {
        //reads incoming messages until delimiter ":"
        msg = uart->readUntil(eom);
        uint32_t duration = 0;
        
        switch(msg.charAt(0)) {
            case 'B': {   //check if input is valid motor input:
                if((msg.charAt(2)-'0') < 0 || (msg.charAt(2)-'0') > 10){
                    uBit.display.scroll(msg);
                    break;
                }
                duration = (uint32_t)((msg.charAt(2)-'0') * 1000);
                duration = duration + (uint32_t)((msg.charAt(4)-'0') * 100);
                moveBot(msg);
                uBit.sleep(duration);
                moveBot("s");
                break;
            }   
            // play Melody
            case 'M': {
                //check if input is valid melody input:
                if((msg.charAt(1)-'0') < 1 || (msg.charAt(1)-'0') > storedSongs){
                    uBit.display.scroll(msg);
                    break;
                }
                int songidx = (int)(msg.charAt(1)-'0') - 1; //song index in SONGS array
                playMelody(songidx);
                break;
            } 
            // change Motorgeschwindigkeit (Motor Velocity)
            case 'G': {
                char motor = msg.charAt(1);
                uint32_t val = (uint32_t)((msg.charAt(2)-'0') * 10 + (msg.charAt(3)-'0'));
                // check if number is valid
                if(val < 1 || val > 16){
                    uBit.display.scroll(msg);
                    break;
                }
                // scale to velocity
                int vel = int((val * 64)-1);
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
                break;
            }  
            // LED Anzeige (show LED picture)
            case 'A': {
                int idx = msg.charAt(1) - '0';
                if(idx > storedPictures){
                    uBit.display.scroll(msg);
                    break;
                }
                int time_to_shine = (msg.charAt(2) - '0') * 1000;
                if(time_to_shine < 0 || time_to_shine > 9000){
                    uBit.display.scroll(msg);
                    break;
                }
                MicroBitImage i((ImageData*)PICTURES[idx-1]);
                uBit.display.print(i,0,0,0,time_to_shine);
                uBit.display.clear();
                break;
            }
            default: {
                uBit.display.scroll(msg);
                break;
            }
        }
        // turn everything off
        moveBot("s");
        MelodyPin.setAnalogValue(0);
        // send confirmation to app
        uart->send("OK\n");
    }
}

void onDisconnected(MicroBitEvent) {
    uBit.display.scroll("D");
}

void onTouch(MicroBitEvent) {
    //<! shows angry face when microbit bumps into something
    if(SensorPinR.isTouched() || SensorPinL.isTouched()){
        MicroBitImage angry_face = ((ImageData*)angry);
        //uBit.display.print(angry_face,5);
        //uBit.display.print(angry_face, 0, 0, 0 , 500);
        uBit.display.print("y");
    }
    else{
        uBit.display.clear();
    }
}

void onButtonB(MicroBitEvent) {
    //<! stops motors when ButtonB is clicked long
    uBit.display.scroll("stop");
    moveBot("s");
}


int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    
    //serial communication via uart
    uBit.serial.baud(9600);
    uBit.serial.send("A\r\n");
    
    //sensor pins
    SensorPinR.eventOn(MICROBIT_PIN_EVENT_ON_TOUCH);
    SensorPinL.eventOn(MICROBIT_PIN_EVENT_ON_TOUCH);
    MelodyPin.setAnalogValue(0);

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_IO_P19, MICROBIT_PIN_EVENT_ON_TOUCH, onTouch);
    uBit.messageBus.listen(MICROBIT_ID_IO_P20, MICROBIT_PIN_EVENT_ON_TOUCH, onTouch);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonB);
    
    uart = new MicroBitUARTService(*uBit.ble,32,32);
    uBit.display.scroll("R4G");
    
    //gives Bluetooth access to the micro:bit LED matrix
    //new MicroBitLEDService(*uBit.ble, uBit.display);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

/***** Motor control: choose your board or write your own motor control *****/

// ElecFreaks Motor:bit Board

void moveBot(ManagedString msg) {
    // Motor 1 PWM = P1,    Motor1 direction = P8 (LOW = CC, HIGH = C)
    // Motor 2 PWM = P2,    Motor2 direction = P12(LOW = CC, HIGH = C)
    // bitmask motor control: X X X X P12 P2 P8 P1
    // bitmasks for motor pins:
    unsigned char m1_pwm = 1, m1_dir = 2, m2_pwm = 4, m2_dir = 8;
    // bitmasks for directions:
    unsigned char bitmasks[6] = {15,5,12,3,13,7};
    ManagedString direction(msg.charAt(1));
    unsigned char moveMask = set_direction(direction,bitmasks);
    //set the pin values according to direction
    uBit.io.P12.setDigitalValue((m2_dir & moveMask)/m2_dir);              //dir motor2
    uBit.io.P8.setDigitalValue((m1_dir & moveMask)/m1_dir);               //dir motor1
    uBit.io.P1.setAnalogValue((m1_pwm & moveMask)* velocity_1);             //pwm motor1
    uBit.io.P2.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2);   //pwm motor2
}



// Keyestudio Motor Driver Board v1.8
/*
void moveBot(ManagedString msg) {
    // Enable = pin14
    // Motor1 CW = pin12,   Motor1 CCW = pin13
    // Motor2 CW = pin15,   Motor2 CCW = pin16
    // Motor1 PWM = pin1,   Motor2 PWM = pin2
    // Bitmask motor control: X X X P15 P16 P12 P13 P14
    // bitmasks for directions:
    unsigned char bitmasks[6] = {11,21,9,3,13,19};
    ManagedString direction(msg.charAt(1));
    unsigned char moveMask = set_direction(direction,bitmasks);
    // set motor pins according to direction
    uBit.io.P1.setAnalogValue(velocity_1);
    uBit.io.P2.setAnalogValue(velocity_2);
    uBit.io.P12.setDigitalValue((4 & moveMask)/4);
    uBit.io.P13.setDigitalValue((2 & moveMask)/2);
    uBit.io.P15.setDigitalValue((16 & moveMask)/16);
    uBit.io.P16.setDigitalValue((8 & moveMask)/8);
    uBit.io.P14.setDigitalValue(1 & moveMask);
}
*/


//Waveshare Motor Driver for micro:bit
/*
// pre configure pwm pins 8 and 16 as analog outputs
MicroBitPin P8(MICROBIT_ID_IO_P8, MICROBIT_PIN_P8, PIN_CAPABILITY_ANALOG);
MicroBitPin P16(MICROBIT_ID_IO_P16, MICROBIT_PIN_P16, PIN_CAPABILITY_ANALOG);
 
void moveBot(ManagedString msg) {
    // Motor A in1 = pin 13,    Motor A in2 = pin 12
    // Motor B in1 = pin 14,    Motor B in2 = pin 15
    // Motor A PWM = pin 8,     Motor B PWM = pin 16
    // bitmask for motor control: 0 0 P16 P15 P14 P8 P12 P13
    // bitmasks for motorPins
    unsigned char m1_pin1 = 1, m1_pin2 = 2, m1_pwm = 4, m2_pin1 = 8, m2_pin2 = 16, m2_pwm = 32;
    // bitmasks for directions:
    unsigned char bitmasks[6] = {15,5,12,3,13,7};
    ManagedString direction(msg.charAt(1));
    unsigned char moveMask = set_direction(direction,bitmasks);
    //set the pin values according to direction
    uBit.io.P13.setDigitalValue(m1_pin1 & moveMask);                //dir motor1
    uBit.io.P12.setDigitalValue((m1_pin2 & moveMask)/m1_pin2);
    uBit.io.P14.setDigitalValue((m2_pin1 & moveMask)/m2_pin1);      //dir motor2
    uBit.io.P15.setDigitalValue((m2_pin2 & moveMask)/m2_pin2);
    //if((P8.setAnalogValue(((m1_pwm & moveMask)/m1_pwm) * velocity)) != MICROBIT_OK){uBit.display.scroll("P8");}
    P8.setAnalogValue(((m1_pwm & moveMask)/m1_pwm) * velocity_1);   //pwm motor1
    P16.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2);  //pwm motor2
}
*/

