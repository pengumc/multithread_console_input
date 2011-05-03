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

using namespace std;
int main(){
//start inputthread
	int key;
	unsigned char running=1;
	CInputThread inputthread(&key, 200);
	inputthread.start();
	while(running){
		if(inputthread.waitForInput(100)==0){
            if(key != 0){
	    		cout << "--" << key << "\n";
		    	if(key == 'q') running=0;
		    }
			inputthread.reset();
		}
	}//main
	cout << "stopping input thread...\n";
	inputthread.stop();
	return 0;
}
