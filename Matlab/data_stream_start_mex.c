#include "mex.h"
#include "matrix.h"
#include "data_stream.h"
#include "file_IO_callbacks.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

void getComPortString(unsigned short portNumber, char *comString, size_t size);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nlhs != 1 || nrhs != 2) {
        mexErrMsgTxt("wrong number of input/output arguments");
        return;
    }

    data_stream_client_instance* instance_ptr = (data_stream_client_instance*) mxMalloc(sizeof(data_stream_client_instance));
    if (instance_ptr == NULL) {
        mexErrMsgTxt("malloc error");
        return;
    }
    mexMakeMemoryPersistent(instance_ptr);

    (*instance_ptr)=(data_stream_client_instance){0};

    unsigned short COM_number = (unsigned short) mxGetScalar(prhs[0]);
    unsigned int baudrate = (unsigned int) mxGetScalar(prhs[1]);

    char comPath[5];
    getComPortString(COM_number, comPath, sizeof(comPath));

    HANDLE hSerial;
    

    hSerial = CreateFile(comPath,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0);
    if(hSerial==INVALID_HANDLE_VALUE){
        if(GetLastError()==ERROR_FILE_NOT_FOUND){
            mexErrMsgTxt("serial port does not exist");
        }
        mexErrMsgTxt("serial port error");
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        mexErrMsgTxt("serial port error getting state");
    }
    
    dcbSerialParams.BaudRate=baudrate;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
        mexErrMsgTxt("serial port error setting state");
    }


   /* COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
         mexErrMsgTxt("serial port error setting timeouts");
    }
    */

    data_stream_client_init(instance_ptr, file_read_callback, file_write_callback, (void*) hSerial, NULL,NULL);

    mxArray* returnval_mx = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    mxUint64* instance_ptr_uint64=mxGetUint64s(returnval_mx);
    *instance_ptr_uint64 = (mxUint64) instance_ptr;

    plhs[0] = returnval_mx;

}

void getComPortString(unsigned short portNumber, char *comString, size_t size) {
    if (portNumber < 1 || portNumber > 255) {
        snprintf(comString, size, "INVALID");
        return;
    }
    snprintf(comString, size, "COM%hu", portNumber); 
}
