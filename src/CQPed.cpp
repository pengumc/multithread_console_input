#ifndef __CQPED__
#define __CQPED__
//quadraped class--------------------------------------------------------------
class CQPed{
    public:
        CQPed(){reset();}
        void reset();
        CServo servoArray[SERVOS];
        CUsbDevice usb;
        void sendToDev();
        void readFromDev();
        void changeServo(uint8_t servo, double a);
};


void CQPed::reset(){
    usb.connect();
    servoArray[2].offset = -PI/2;
    servoArray[2].direction = -1;
}

void CQPed::changeServo(uint8_t servo, double a){
    servoArray[servo].setAngle(servoArray[servo].angle + a);
    sendToDev();
}

void CQPed::sendToDev(){
    usb.sendServoData(servoArray);
}

void CQPed::readFromDev(){
    usb.readServoData(servoArray);
}
#endif
