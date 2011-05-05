#define USBCOMMANDLINE
#define USBRETRY 3
#include <usb.h>
#include "opendevice.h"
#include "requests.h"
#include "usbconfig.h"
#include "i2c_header.h"
//------------------------------------------------------------------------------
class CUsbDevice{
    public:
        CUsbDevice();
        uint8_t connected;
        uint8_t connect();
        void readServoData();
        void sendServoData();
        void printA();
        void printB();

//    private:
        usb_dev_handle *handle;
        char bufferA[BUFLEN_SERVO_DATA];
        char bufferB[BUFLEN_SERVO_DATA];
        char vendor[USB_CFG_VENDOR_NAME_LEN+1];
        char product[USB_CFG_DEVICE_NAME_LEN+1];
        unsigned char rawVid[2];
        unsigned char rawPid[2];
        int vid,pid;
        int sendCtrlMsg(int request, int reqType, int wval, int wind, char *buffer);
};

CUsbDevice::CUsbDevice(){
    vendor = {USB_CFG_VENDOR_NAME,0};
    product = {USB_CFG_DEVICE_NAME,0};
    rawVid = {USB_CFG_VENDOR_ID};
    rawPid = {USB_CFG_DEVICE_ID};
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    bufferA = SERVO_DATA_EMPTY;
    bufferB = SERVO_DATA_EMPTY;
    usb_init();
}

void CUsbDevice::printA(){
    char i;
    printf("buffer A: {");
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("%d,",bufferA[i]);
    }
    printf("}\n");
}
void CUsbDevice::printB(){
    char i;
    printf("buffer B: {");
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("%d,",bufferB[i]);
    }
    printf("}\n");
}

uint8_t CUsbDevice::connect(){
    if (usbOpenDevice(&handle, vid, vendor, pid, product,
        NULL, NULL, NULL) != 0){
        fprintf(
            stderr,
            "USB device \"%s\" with vid=0x%x pid=0x%x\n  NOT FOUND\n",
            product, vid, pid);
        connected = 0;
    }else connected = USBRETRY;
    return connected;
}

void CUsbDevice::readServoData(){
    int i; 
    if (sendCtrlMsg(CUSTOM_RQ_LOAD_POS_FROM_I2C, USB_ENDPOINT_IN,0,0,bufferA)==0){;
        i=sendCtrlMsg(CUSTOM_RQ_GET_POS, USB_ENDPOINT_IN,0,0,bufferA);
        if(i>0){
            printf("received %d {",i);
            for (i=0;i<BUFLEN_SERVO_DATA;i++){
                printf("%d,",bufferA[i]);
            }
            printf("}\n");
        }
        //store in servo's
    }else printf("readServoData: device was not ready\n");
}

void CUsbDevice::sendServoData(){
    int i;
    i = sendCtrlMsg(CUSTOM_RQ_SET_DATA, USB_ENDPOINT_OUT, 0,0,bufferA);
    printf("sendServoData: %d\n",i);
}

int CUsbDevice::sendCtrlMsg(int request, int reqType, int wval, int wind, char *buffer){
    int cnt;
    unsigned char i;
    if(handle && connected){
        cnt = usb_control_msg(
            handle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | reqType,
            request,
            wval,
            wind,
            buffer,
            BUFLEN_SERVO_DATA,
            1000
        );
        if (cnt < 0){
            fprintf(stderr, "usb_control_msg: %s\n",usb_strerror());
        } else connected = USBRETRY;
    }else printf("sendCtrlMsg: not connected...\n");
    return cnt;
}
