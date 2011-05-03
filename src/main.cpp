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


using namespace std;

void setupMaple(CMaple* m){
    char statement[100];
    sprintf(statement, "xpos:=3+6.5*cos(b)+4*cos(c);");
    m->eval(statement);
    sprintf(statement, "ypos:=6.5*sin(b)+4*sin(c);");
    m->eval(statement);
}

void calculateAngles(CMaple* m, double x, double y){
    char statement[1024];
    sprintf(statement,
        "RealDomain:-solve({xpos=%f,ypos=%f},[b,c],UseAssumptions=true) assuming -Pi<=b,b<=Pi, -Pi<=c,c<=Pi;",
        x,y);
    m->eval(statement);
}

int main(int argc, char *argv[]){
    int key =0;
    unsigned char running=1;
    double X,Y;
    X = 9.5;
    Y = -4;

    //create thread
    CInputThread inputthread(&key, 200);
    inputthread.start();
    //create maple object
    CMaple maple;
    maple.start();
    setupMaple(&maple);
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
                    calculateAngles(&maple, X,Y);
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
                case 'p':
                    printf("X = %f\nY= %f\n",X,Y);
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
    maple.stop();
    return 0;
}
