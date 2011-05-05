//windows: g++ test.cpp -o test.exe
//linux g++ test.cpp -o test -lpthread
//------------------------------------------------------------------------------
#include <iostream>
#include <stdint.h>
#include <math.h>
#ifdef _WIN32
    #include <conio.h>
    #include <win32_input.cpp>
#else
    #include <stdio.h>
    #include <pthreads_input.cpp>
#endif

#include "CMaple.cpp"
#include "CPosCalc.cpp"
#include "CUsbDevice.cpp"
#include "CQPed.cpp"

using namespace std;


int main(int argc, char *argv[]){
    int key =0;
    unsigned char running=1;
    uint8_t servo=0;
    double X,Y;
    X = 9.5;
    Y = -4;

    //create thread
    CInputThread inputthread(&key, 200);
    inputthread.start();
    //create maple object
    CPosCalc poscalc;
    poscalc.setup();
    //quadraped
    CQPed quadraped;
    printf("%e\n",quadraped.servoArray[2].pulsewidthToAngle(72));
    printf("%e\n",quadraped.servoArray[2].angle);
    printf("%d\n",quadraped.servoArray[2].angleToPulsewidth(.1));
    printf("%d\n",quadraped.servoArray[2].pulsewidth);
    //main loop
    while(running){
        //wait for a keypress
        if(inputthread.waitForInput(100)==0){
            if(key != 0){
                //a key was pressed, do stuff
                switch(key){
                case 'q':
                    running=0; //q quits the program
                    break;
                case 'a':
                    poscalc.calculateAngles(X,Y);
                    break;
                case '1':
                    X+=0.5;
                    break;
                case '2':
                    X-=0.5;
                    break;
                case '3':
                    Y+=0.5;
                    break;
                case '4':
                    Y-=0.5;
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
                    printf("X = %f\nY= %f\n",X,Y);
                    quadraped.usb.printA();
                    quadraped.usb.printB();
                    break;
                case 'r':
                    quadraped.readFromDev();
                    break;
                case 's':
                    quadraped.sendToDev();
                    break;
                default:
                    printf("--%c\n",key);
                }
            }
            inputthread.reset();
        }
    }//mainloop
    cout << "stopping input thread...\n";
    inputthread.stop();
    cout << "stopping maple...\n";
    poscalc.stop();
    return 0;
}
