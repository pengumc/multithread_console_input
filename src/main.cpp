//windows: g++ test.cpp -o test.exe
//linux g++ test.cpp -o test -lpthread
//------------------------------------------------------------------------------
#define __MAIN__
#define PI 3.14159265
#include <iostream>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#ifdef _WIN32
    #include <conio.h>
    #include <win32_input.cpp>
		#define usleep(x) Sleep(x/1000)
#else
    #include <pthreads_input.cpp>
#endif

#include "CSolver.cpp"
#include "CServo.cpp"
#include "CUsbDevice.cpp"
#include "CQPed.cpp"
#include "CAngle.cpp"

using namespace std;


int main(int argc, char *argv[]){
    int key =0;
    unsigned char running=1;
    uint8_t servo=0;

    //create thread
    CInputThread inputthread(&key, 200);
    inputthread.start();
    //quadraped
    CQPed quadraped;
    printf("%e\n",quadraped.servoArray[2].pulsewidthToAngle(72));
    printf("%d\n",quadraped.servoArray[2].angleToPulsewidth(.1));
    //main loop
    #define SPEED 0.2
    while(running){
        //wait for a keypress
        if(inputthread.waitForInput(1000)==0){
            if(key != 0){
                //a key was pressed, do stuff
                switch(key){
                case 'q':
                    running=0; //q quits the program
                    break;
                case 'c':
                    break;
                case 'w':
                    quadraped.moveRelative(0, -SPEED,0);
                    quadraped.sendToDev();
                    break;
                case 's':
                    quadraped.moveRelative(0, SPEED,0);
                    quadraped.sendToDev();
                    break;
                case 'a':
                    quadraped.moveRelative(SPEED,0,0);
                    quadraped.sendToDev();
                    break;
                case 'd':
                    quadraped.moveRelative(-SPEED, 0,0);
                    quadraped.sendToDev();
                    break;
               case '1':
                    break;
                case '2':
                    break;
                case '3':
                    break;
                case '4':
                    break;
                case '!':
                    servo = 1;
                    break;
                case '@':
                    servo = 2;
                    break;
                case '#':
                    servo = 3;
                    break;
                case '$':
                    servo = 4;
                    break;
                case '%':
                    servo = 5;
                    break;
                case ')':
                    servo = 0;
                    break;
                case '+':
                    quadraped.changeServo(servo, 0.1);
                    break;
                case '-':
                    quadraped.changeServo(servo, -0.1);
                    break;
                case 'p':
                    quadraped.printPos();
                    quadraped.usb.printA();
                    quadraped.usb.printB();
                    quadraped.printAngles();
                    break;
                case 'r':
                    quadraped.readFromDev();
                    break;
                case 'R':
                    quadraped.usb.getData();
                    break;
                case 'v':
                    quadraped.sendToDev();
                    break;
                case '`':
                    quadraped.reset();
                    quadraped.sendToDev();
                    break;
                default:
                    printf("--%c\n",key);
                }
            }
            inputthread.reset();
        }else{//input wait
#define SENS 32
#define DIV 4000
#define sleepytime 300
            char trigger=0;
            if (quadraped.usb.connected>0){
            quadraped.usb.getData();
            //right stick, incremental control
            int8_t temp = ((uint8_t)quadraped.usb.bufferB[6]-128);
            if (abs(temp) > SENS) {
                trigger=1;
                printf("%d\n",temp);
                quadraped.moveRelative(0, ((float)temp)/DIV,0);
            }
            temp = ((uint8_t)quadraped.usb.bufferB[5]-128);
            if (abs(temp) > SENS) {
                trigger=1;
                printf("%d\n",temp);
                quadraped.moveRelative(-((float)temp)/DIV,0,0);
            }
            //left stick, absolute control
            temp =  ((uint8_t)quadraped.usb.bufferB[7]-128);
            if(!(quadraped.usb.bufferB[2] & 8)){
                trigger=1;
                quadraped.x[0] = 9.0 - ((float)temp)/64;
                quadraped.x[1] = -8 - ((float)temp)/64;
                quadraped.moveRelative(0,0,0);
            }
            temp =  ((uint8_t)quadraped.usb.bufferB[8]-128);
            if(!(quadraped.usb.bufferB[2] & 8)){//R1
                trigger=1;
                quadraped.y[0] = - 5.5 + (float)(temp/64);
                quadraped.y[1] = - 5.5 + (float)(temp/64);
                quadraped.moveRelative(0,0,0);
            }
            if(trigger){
                quadraped.sendToDev();
                trigger=0;
                usleep(sleepytime);
            }
            }
        }
    }//mainloop
    cout << "stopping input thread...\n";
    inputthread.stop();
    usleep(100);
    return 0;
}
