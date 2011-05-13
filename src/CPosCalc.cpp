//------------------------------------------------------------------------------

///Calculates the necessary servo angles from an XY position.
class CPosCalc : public CMaple{
    public:
        ///Start maple and setup the equations needed.
        void setup();
        ///Start maple and setup the equations needed, A,B,C leglengths.
        void setup(double *A, double *B, double *C, uint8_t number_of_legs);
        ///Calculate the angles, returns the amount of solutions.
        uint8_t calculateAngles(double x, double y, uint8_t leg);
        uint8_t calculateAngles(double x, double y, double z, uint8_t leg);
        //Stores the solutions for the alpha angle.
        double alpha[2];
        //Stores the solutions for the beta angle.
        double beta[2];
        //Stores the solutions for the gamma angle.
        double gamma[2];
    private:
        char cmd[1024];
};

void CPosCalc::setup(){
    start();
    sprintf(cmd, "xpos0:=3+6.5*cos(b)+5.5*cos(c);");
    eval(cmd);
    sprintf(cmd, "ypos0:=6.5*sin(b)+5.5*sin(c);");
    eval(cmd);
    sprintf(cmd, "xpos1:=-3+5*cos(b)+5.5*cos(c);");
    eval(cmd);
    sprintf(cmd, "ypos1:=5*sin(b)+5.5*sin(c);");
    eval(cmd);
}

void CPosCalc::setup(double *A, double *B, double *C, uint8_t number_of_legs){
    char i;
    start();
    for (i=0;i<number_of_legs;i++){
        sprintf(cmd, "xpos%d:=cos(a)*((%f)+(%f)*cos(b)+(%f)*cos(c));",
            i, A[i], B[i], C[i]);
        //printf(cmd);
        eval(cmd);
        sprintf(cmd, "ypos%d:=(%f)*sin(b)+(%f)*sin(c);",
            i, B[i], C[i]);
        //printf(cmd);
        eval(cmd);
        sprintf(cmd, "zpos%d:=sin(a)*((%f)+(%f)*cos(b)+(%f)*cos(c));",
            i, A[i], B[i], C[i]);
        //printf(cmd);
        eval(cmd);
    }
}

uint8_t CPosCalc::calculateAngles(double x, double y, double z, uint8_t leg){
    sprintf(cmd,
    "temp:= RealDomain:-solve({xpos%d=%f,ypos%d=%f,zpos%d=%f},[a,b,c],UseAssumptions=true) assuming -Pi<=b,b<=Pi, -Pi<=c,c<=Pi;",
    leg, x, leg, y, leg, z);
    eval(cmd);
    //check number of solutions
    sprintf(cmd, "nops(temp):");
    eval(cmd); //stores in a
    uint8_t resultSize = MapleToInteger8(kv, a);
    printf("calc: %d results\n",resultSize);
    if (resultSize > 0){
        sprintf(cmd, "rhs(temp[1][1]):");
        alpha[0] = MapleToFloat64(kv, eval(cmd));
        sprintf(cmd, "rhs(temp[1][2]):");
        beta[0] = MapleToFloat64(kv, eval(cmd));
        sprintf(cmd, "rhs(temp[1][3])-rhs(temp[1][2]):");
        gamma[0] = MapleToFloat64(kv, eval(cmd));
        if (resultSize > 1){
            sprintf(cmd, "rhs(temp[2][1]):");
            alpha[1] = MapleToFloat64(kv, eval(cmd));
            sprintf(cmd, "rhs(temp[2][2]):");
            beta[1] = MapleToFloat64(kv, eval(cmd));
            sprintf(cmd, "rhs(temp[2][3])-rhs(temp[2][2]):");
            gamma[1] = MapleToFloat64(kv, eval(cmd));
        }else{
            alpha[1] = alpha[0];
            beta[1] = beta[0];
            gamma[1] = gamma[0];
        }
    }
    printf("%f\n%f\n%f\n%f\n%f\n%f\n", alpha[0], beta[0], gamma[0], alpha[1], beta[1], gamma[1]);
    return resultSize;
}

uint8_t CPosCalc::calculateAngles(double x, double y, uint8_t leg){
        sprintf(cmd,
        "temp:= RealDomain:-solve({xpos%d=%f,ypos%d=%f},[b,c],UseAssumptions=true) assuming -Pi<=b,b<=Pi, -Pi<=c,c<=Pi;",
        leg,x,leg,y);
        eval(cmd);
        //check temp for emptiness
        sprintf(cmd, "nops(temp):");
        eval(cmd); //stores in a
        uint8_t resultSize = MapleToInteger8(kv, a);
        printf("calc: %d results\n",resultSize);
        if (resultSize > 0){
            sprintf(cmd,"rhs(temp[1][1]):");
            beta[0] = MapleToFloat64(kv,eval(cmd));
            if (leg%2==1 && beta[0] > 0) beta[0] -= 2*PI;
            sprintf(cmd,"rhs(temp[1][2]-rhs(temp[1][1])):");
            gamma[0] = MapleToFloat64(kv,eval(cmd));
//            if (leg%2==1) gamma[0] *= -1;
            if (leg%2==1 && gamma[0] > PI) gamma[0] -= 2*PI;
            if (resultSize > 1){
                sprintf(cmd,"rhs(temp[2][1]):");
                beta[1] = MapleToFloat64(kv,eval(cmd));
                if (leg%2==1 && beta[1] > 0) beta[1] -= 2*PI;
                sprintf(cmd,"rhs(temp[2][2])-rhs(temp[2][1]):");
                gamma[1] = MapleToFloat64(kv,eval(cmd));
                if (leg%2==1 && gamma[1] > PI) gamma[1] -= 2*PI;
            }else{
                beta[1]=beta[0];
                gamma[1]=gamma[0];
            }
        }
        return resultSize;
}

