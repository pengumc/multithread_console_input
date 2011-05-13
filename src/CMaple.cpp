//------------------------------------------------------------------------------
#include "maplec.h" //from the include directory of your maple installation
///Prints any callbacks to stdout
static void M_DECL textCallBack(void *data, int tag, char *output){
    printf("%s\n",output);
}

///This class provides access to the maple kernel.
class CMaple{
    public:
        ///Stop maple.
        void stop();
        ///Start maple, returns 0 on success.
        int start();
        ///evaulate a maple statement, returns the output.
        ALGEB eval(char *statement);
        CMaple();
    private:
        MCallBackVectorDesc cb;
        char err[2048];
    protected:
        ///stores the result of the last evaluated statement
        ALGEB a; 
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

/** Use ':' instead of ';' at the end of a statement to prevent printing the output
* to stdout.
*/
ALGEB CMaple::eval(char *statement){
    a = EvalMapleStatement(kv, statement);
    return a;
}
