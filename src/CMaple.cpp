//------------------------------------------------------------------------------
#include "maplec.h" //from the include directory of your maple installation
static void M_DECL textCallBack(void *data, int tag, char *output){
    printf("%s\n",output);
}


class CMaple{
    public:
        void stop();
        int start();
        ALGEB eval(char *statement);
        CMaple();
    private:
        MCallBackVectorDesc cb;
        char err[2048];
    protected:
        ALGEB a; //stores last eval result
        MKernelVector kv;
};

CMaple::CMaple(){
    cb = {
        textCallBack, 
        0,   /* errorCallBack not used */
        0,   /* statusCallBack not used */
        0,   /* readLineCallBack not used */
        0,   /* redirectCallBack not used */
        0,   /* streamCallBack not used */
        0,   /* queryInterrupt not used */
        0};  /* callBackCallBack not used */
    kv = NULL;
}

void CMaple::stop(){
    StopMaple(kv);
}

int CMaple::start(){
    kv = StartMaple(1, NULL, &cb, NULL, NULL,  err);
    if(kv == NULL) {
        printf("failed to start, %s\n",err);
        return(1);
    }else return(0);
}

ALGEB CMaple::eval(char *statement){
    a = EvalMapleStatement(kv, statement);
    return a;
}
