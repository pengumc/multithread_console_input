#define PI 3.14159265
#define SERVOS 12

//servo class------------------------------------------------------------------
class CServo{
    public:
        CServo();
        uint8_t maxPulse;
        uint8_t minPulse;
        uint8_t midPulse;
        double direction;
        double offset;
        double pulsewidthToAngle();
        double pulsewidthToAngle(uint8_t pw);
        char setAngle(double a);
        double K;
        uint8_t angleToPulsewidth();
        uint8_t angleToPulsewidth(double a);
        double angle;
        uint8_t pulsewidth;
        friend class CUsbDevice;
};
//for servo 2 and 5: direction = -1, offset = -PI/2
CServo::CServo(){
    angle=0;
    maxPulse = 96;
    minPulse = 48;
    midPulse = 72;
    direction = 1.0;
    offset = 0;
    K = 0.034;
    pulsewidth = angleToPulsewidth();
}

char CServo::setAngle(double a){
//return 0 on success, 1 if maxed
    char tempPW;
    if (a > angleToPulsewidth(PI)) {
        pulsewidth = maxPulse;
        angle = pulsewidthToAngle(maxPulse);
    }else if (a < angleToPulsewidth(-PI)){
        pulsewidth = minPulse;
        angle = pulsewidthToAngle(minPulse);
    }else{
        pulsewidth = angleToPulsewidth(a);
        angle = a;
    }
   
       
}

uint8_t CServo::angleToPulsewidth(){
    return ((angle-offset / K) / direction) + midPulse;
}

uint8_t CServo::angleToPulsewidth(double a){
    return ((a-offset / K) / direction) +midPulse;
}

double CServo::pulsewidthToAngle(){
    return (pulsewidth - ((minPulse+maxPulse)/2)) * direction * K + offset;
}

double CServo::pulsewidthToAngle(uint8_t pw){
    return (pw- midPulse) * direction * K + offset;
}


//quadraped class--------------------------------------------------------------


class CQPed{
    public:
        CQPed(){reset();}
        void reset();
        uint8_t checkAngle(double angle, uint8_t servo);
        CServo servoArray[SERVOS];
};


void CQPed::reset(){

}

uint8_t CQPed::checkAngle(double angle, uint8_t servo){
//    switch(servo){
 //       case 0:

    
}

