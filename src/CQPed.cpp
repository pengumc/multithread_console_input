#ifndef __CQPED__
#define __CQPED__
//quadraped class--------------------------------------------------------------
class CQPed{
    public:
        CQPed(){reset();}
        void reset();
        CServo servoArray[SERVOS];
        CUsbDevice usb;
        CPosCalc poscalc;
        void printPos();
        void moveRelative(double X, double Y);
        void sendToDev();
        void readFromDev();
        void changeServo(uint8_t servo, double a);
        void printAngles();
        uint8_t calcAngles(uint8_t leg);
        void assignBestAngles(
            uint8_t betaServo, uint8_t gammaServo, uint8_t leg);
        double x[2];
        double y[2];
};


void CQPed::reset(){
    poscalc.setup();
    usb.connect();
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    servoArray[2].offset = -(PI/2);
    servoArray[2].flipDirection();
    servoArray[2].setAngle(-PI/2);
    servoArray[4].offset = -PI;
    servoArray[4].setAngle(-PI);
    servoArray[5].offset = -(PI/2);
    servoArray[5].flipDirection();
    servoArray[5].setAngle(-PI/2);
    x[0] = 9.5;
    x[1] = -8;
    y[0] = -5.5;
    y[1] = 5.5;
    printf("4: max angle=%f\n",servoArray[4].pulsewidthToAngle(96));
    printf("4: min angle=%f\n",servoArray[4].pulsewidthToAngle(48));
    printf("4: isValid %d\n",servoArray[5].isValidAngle(-1.8));

}

void CQPed::moveRelative(double X, double Y){
    x[0] += X;
    x[1] -= X;
    y[0] += Y;
    y[1] += Y;
    printf("=== leg 0:\n");
    if (calcAngles(0)) assignBestAngles(1,2,0);
    printf("=== leg 1:\n");
    if (calcAngles(1)) assignBestAngles(4,5,1);
}

void CQPed::printPos(){
    printf("x0 = %f\ny0 = %f\nx1 = %f\ny1 = %f\n",x[0],y[0],x[1],y[1]);
}

void CQPed::assignBestAngles(
    uint8_t betaServo, uint8_t gammaServo, uint8_t leg){
    
    double b = servoArray[betaServo].angle;
    double c = servoArray[gammaServo].angle;
    double bDiff0=PI;
    double bDiff1=PI;
    //first result
    if (servoArray[betaServo].isValidAngle(poscalc.beta[0])){
        if (servoArray[gammaServo].isValidAngle(poscalc.gamma[0])){
            bDiff0 = b - poscalc.beta[0];
        }
    }
    //second result
    if (servoArray[betaServo].isValidAngle(poscalc.beta[1])){
        if (servoArray[gammaServo].isValidAngle(poscalc.gamma[1])){
            bDiff1 = b - poscalc.beta[1];
        }
    }
    if (bDiff0 == bDiff1 && bDiff1 == PI){
        //both invalid
        printf("no valid angles available\n");
        printf("%f\n",poscalc.beta[0]);
        printf("%f\n",poscalc.gamma[0]);
        printf("%f\n",poscalc.beta[1]);
        printf("%f\n",poscalc.gamma[1]);
    } else if (fabs(bDiff0) <= fabs(bDiff1)){
        printf("selected 0\n");
        printf("%f --> %d\n", poscalc.beta[0], 
            servoArray[betaServo].angleToPulsewidth(poscalc.beta[0]));
        printf("%f --> %d\n", poscalc.gamma[0], 
            servoArray[gammaServo].angleToPulsewidth(poscalc.gamma[0]));
        servoArray[betaServo].setAngle(poscalc.beta[0]);
        servoArray[gammaServo].setAngle(poscalc.gamma[0]);
    } else {
        printf("selected 1\n");
        printf("%f --> %d\n", poscalc.beta[1], 
            servoArray[betaServo].angleToPulsewidth(poscalc.beta[1]));
        printf("%f --> %d\n", poscalc.gamma[1], 
            servoArray[gammaServo].angleToPulsewidth(poscalc.gamma[1]));
        servoArray[betaServo].setAngle(poscalc.beta[1]);
        servoArray[gammaServo].setAngle(poscalc.gamma[1]);
    }
}

uint8_t CQPed::calcAngles(uint8_t leg){
    return poscalc.calculateAngles(x[leg],y[leg], leg);
}


void CQPed::printAngles(){
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("servo %d = %f\n",i,servoArray[i].angle);
    }
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
