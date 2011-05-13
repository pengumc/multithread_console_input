//-----------------------------------------------------------------------------
#include <stdint.h>
#include "CAngle.cpp"
#include <stdlib.h>
//#include "CUsbDevice.cpp"

class CServo2{
    friend class CUsbDevice;
    public:
        CServo2();
        void reset();
        double offset;
        double pulsewidthToAngle(); 
        double pulsewidthToAngle(uint8_t s); 
        uint8_t isValid(double s);
        void setAngle(double s);
        void changeAngle(double s);
//TODO pw <-> angle
    private:
        double K;
        uint8_t midPulse;
        uint8_t minPulse;
        uint8_t maxPulse;
        double direction;
        CAngle angle;
        uint8_t pw;
};

CServo2::CServo2(){reset();}

void CServo2::reset(){
    minPulse = 48;
    maxPulse = 96;
    midPulse = (minPulse + maxPulse)/2;
    direction=1.0;
    offset = 0;
    K = 0.034;
    angle = 0.0;
    pw = midPulse;
    printf("mid %d\npw %d\nangle %f\n", midPulse, pw, angle.get());
}


double CServo2::pulsewidthToAngle(){
    return (pw - (midPulse)) * direction * K + offset;
    
}

double CServo2::pulsewidthToAngle(uint8_t s){
    return (s - (midPulse)) * direction * K + offset;

}

uint8_t CServo2::isValid(double s){
    return 0;
}

void CServo2::setAngle(double s){
    
}

void CServo2::changeAngle(double s){

}

int main(int argc, char *argv[]){
    CServo2 S;
    printf("%f\n",S.pulsewidthToAngle(atoi(argv[1])));

}

