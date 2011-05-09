//------------------------------------------------------------------------------

class CPosCalc : public CMaple{
    public:
       void setup();
       uint8_t calculateAngles(double x, double y, uint8_t leg);
       double beta[2];
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

