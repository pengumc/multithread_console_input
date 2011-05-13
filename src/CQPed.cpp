#ifndef __CQPED__
#define __CQPED__
//quadraped class--------------------------------------------------------------
#define Q_LEGS 2
///quadraped device, currently in 2 leg mode.
class CQPed{
    public:
        CQPed(){reset();}
        ///reconnect and reset the entire thing.
        void reset();
        ///array of servos, 3 per leg.
        CServo servoArray[SERVOS];
        ///the usb helper.
        CUsbDevice usb;
        ///Maple powered position calculator
        CPosCalc poscalc;
        ///prints the x and y positions of all legs.
        void printPos();
        ///change the x and y position of the center body.
        void moveRelative(double X, double Y);
        ///send the servo states to the physical device.
        void sendToDev();
        ///read servo states from physical device.
        void readFromDev();
        ///change the angle of a single servo by a.
        void changeServo(uint8_t servo, double a);
        ///print the servo angles from memory.
        void printAngles();
        ///calculate the angles needed for the position specified by x[] and y[]
        uint8_t calcAngles(uint8_t leg);
        ///chose the best solution and assign it to the servos
        void assignBestAngles(
            uint8_t betaServo, uint8_t gammaServo, uint8_t leg);
        ///x positions per leg
        double x[2];
        ///y positions per leg
        double y[2];
        ///z positions per leg
        double z[2];
        ///length of leg segment 1 (top) per leg.
        double A[Q_LEGS];
        ///length of leg segment 2 (middle) per leg.
        double B[Q_LEGS];
        ///length of leg segment 3 (bottom) per leg.
        double C[Q_LEGS];
        void assignBestAngles3D(
    uint8_t alphaServo, uint8_t betaServo, uint8_t gammaServo, uint8_t leg);
};

/** Most default values are hardcoded into this function.
*/
void CQPed::reset(){
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
    y[0] = 5.5;
    y[1] = 5.5;
    z[0] = 0;
    z[1] = 0;
    //leg 0
    A[0] = 3;
    B[0] = 6.5;
    C[0] = 5.5;
    //leg 1
    A[1] = 3;
    B[1] = 5;
    C[1] = 5.5;
    poscalc.setup(A, B, C, Q_LEGS);
    //printf("4: max angle=%f\n",servoArray[4].pulsewidthToAngle(96));
    //printf("4: min angle=%f\n",servoArray[4].pulsewidthToAngle(48));
    //printf("4: isValid %d\n",servoArray[5].isValidAngle(-1.8));

}

void CQPed::moveRelative(double X, double Y){
    x[0] += X;
    x[1] -= X;
    y[0] += Y;
    y[1] += Y;
    printf("=== leg 0:\n");
    if (calcAngles(0)) assignBestAngles(1,2,0);
    else {
        x[0] -= X;
        x[1] += X;
        y[0] -= Y;
        y[1] -= Y;
        return;
    }
    printf("=== leg 1:\n");
    if (calcAngles(1)) assignBestAngles(4,5,1);
    else {
        x[0] -= X;
        x[1] += X;
        y[0] -= Y;
        y[1] -= Y;
        return;
    }
}

void CQPed::printPos(){
    printf("x0 = %f\ny0 = %f\nx1 = %f\ny1 = %f\n",x[0],y[0],x[1],y[1]);
}

void CQPed::assignBestAngles3D(
    uint8_t alphaServo, uint8_t betaServo, uint8_t gammaServo, uint8_t leg){

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
    return poscalc.calculateAngles(x[leg],y[leg], z[leg], leg);
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
