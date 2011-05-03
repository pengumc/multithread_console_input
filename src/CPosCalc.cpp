//------------------------------------------------------------------------------

class CPosCalc : public CMaple{
    public:
       void setup();
       void calculateAngles(double x, double y);
       double beta;
       double gamma;
    private:
        char cmd[1024];
};

void CPosCalc::setup(){
    start();
    sprintf(cmd, "xpos:=3+6.5*cos(b)+4*cos(c);");
    eval(cmd);
    sprintf(cmd, "ypos:=6.5*sin(b)+4*sin(c);");
    eval(cmd);
}

void CPosCalc::calculateAngles(double x, double y){
        sprintf(cmd,
        "temp:= RealDomain:-solve({xpos=%f,ypos=%f},[b,c],UseAssumptions=true) assuming -Pi<=b,b<=Pi, -Pi<=c,c<=Pi;",
        x,y);
        eval(cmd);
        //check temp for emptiness
        sprintf(cmd, "nops(temp):");
        eval(cmd); //stores in a
        uint8_t resultSize = MapleToInteger8(kv, a);
        if (resultSize > 1){;
            sprintf(cmd,"rhs(temp[2][1]);");
            beta = MapleToFloat64(kv,eval(cmd));
            sprintf(cmd,"rhs(temp[2][2]-rhs(temp[2][1]));");
            gamma = MapleToFloat64(kv,eval(cmd));
            //TODO determine best option when there's >1 results
        }
}
