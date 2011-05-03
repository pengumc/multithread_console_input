//-lm -lc -I/home/michiel/maple13/extern/include/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "maplec.h"

static void M_DECL textCallBack(void *data, int tag, char *output){
    printf("%s\n",output);
}



int main(int argc, char *argv[]){
    char err[2048];

    MKernelVector kv = NULL;
    MCallBackVectorDesc cb = {
        textCallBack, 
        0,   /* errorCallBack not used */
        0,   /* statusCallBack not used */
        0,   /* readLineCallBack not used */
        0,   /* redirectCallBack not used */
        0,   /* streamCallBack not used */
        0,   /* queryInterrupt not used */
        0    /* callBackCallBack not used */
    };      
    
    ALGEB r, l;
    //start maple
    kv = StartMaple(argc, argv, &cb, NULL, NULL,  err);
    if(kv == NULL) {
        printf("failed to start, %s\n",err);
        return(1);
    }
    char i;
    char cmd[1024];
        sprintf(cmd,"RealDomain:-solve({3+6.5*cos(b)+4*cos(c+b)=9.5, 6.5*sin(b) + 4*sin(c+b)=-4},[b,c]);");
    r = EvalMapleStatement( kv, "xpos:=3+6.5*cos(b)+4*cos(c);");
    r = EvalMapleStatement( kv, "ypos:=6.5*sin(b)+4*sin(c);");
    r = EvalMapleStatement( kv, "RealDomain:-solve({xpos=9.5,ypos=-4},[b,c],UseAssumptions=true) assuming -Pi<=b,b<=Pi, -Pi<=c,c<=Pi;");
    r = EvalMapleStatement(kv, "RealDomain:-solve({6.5*sin(b)+4*sin(c+b) = -4, 3+6.5*cos(b)+4*cos(c+b) = 9.5}, [b, c]);");
    MapleALGEB_Printf(kv,"%a\n",r);
    StopMaple(kv);
}



