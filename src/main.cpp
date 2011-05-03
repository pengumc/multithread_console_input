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

#include "mapleClass.h"

using namespace std;
int main(){
//start inputthread
    int key =0;
    unsigned char running=1;
    //create thread
    CInputThread inputthread(&key, 200);
    inputthread.start();
    //main loop
    while(running){
        //wait for a keypress
        if(inputthread.waitForInput(100)==0){
            if(key != 0){
                //a key was pressed, do stuff
                cout << "--" << key << "\n";
                if(key == 'q') running=0; //q quits the program
            }
            inputthread.reset();
        }
    }//mainloop
    cout << "stopping input thread...\n";
    inputthread.stop();
    return 0;
}
