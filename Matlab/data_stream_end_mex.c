#include "mex.h"
#include "matrix.h"
#include "data_stream.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nlhs != 0 || nrhs != 1) {
		mexErrMsgTxt("wrong number of input/output arguments");
	}
	const mxArray* instance_ptr_mx = prhs[0];
    mxUint64* instance_ptr_uint64=mxGetUint64s(instance_ptr_mx);

	if (instance_ptr_uint64 == NULL) {
		mexErrMsgTxt("instance is not valid object");
	}
    
	data_stream_client_instance* instance = (data_stream_client_instance*)*instance_ptr_uint64;

	if (instance == NULL) {
		mexErrMsgTxt("instance is not valid object");
	}

    HANDLE hSerial =(HANDLE)(instance->file_stream.handle);
    CloseHandle(hSerial);
    
	mxFree(instance);

}
