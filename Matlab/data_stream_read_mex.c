#include "mex.h"
#include "matrix.h"
#include "data_stream.h"


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	if (nlhs != 2 || nrhs != 3) {
		mexErrMsgTxt("wrong number of input/output arguments");
	}
	const mxArray* instance_ptr_mx = prhs[0];
	const mxArray* stream_id_mx = prhs[1];
    const mxArray* data_size_mx = prhs[2];

	if (!mxIsNumeric(stream_id_mx)) {
		mexErrMsgTxt("param must be numeric type");
	}
   
	 mxUint64* instance_ptr_uint64=mxGetUint64s(instance_ptr_mx);      

	if (instance_ptr_uint64 == NULL) {
		mexErrMsgTxt("instance is not valid object");
	}
    
	data_stream_client_instance* instance = (data_stream_client_instance*)*instance_ptr_uint64;

	if (instance == NULL) {
		mexErrMsgTxt("instance is not valid object");
	}


	if (!mxIsDouble(stream_id_mx)||!mxIsScalar(stream_id_mx)) {
		mexErrMsgTxt("stream ID is not double scalar");
	}

    float timestamp=0.0;
    size_t data_size=(size_t)mxGetScalar(data_size_mx);
    mxArray* return_value_mx = mxCreateNumericMatrix(data_size, 1, mxSINGLE_CLASS, mxREAL);
    float* data_ptr = (float*) mxGetData(return_value_mx);


	data_stream_return_t return_data_stream = data_stream_client_read_stream(instance, (data_stream_id_t) mxGetScalar(stream_id_mx),data_ptr,data_size*sizeof(float));
    timestamp=instance->data_struct.timestamp;
    mxArray* timestamp_mx = mxCreateDoubleScalar(timestamp);
	
	if (return_data_stream != data_stream_ok)
    {

		switch (return_data_stream) {
		case data_stream_error:
			mexErrMsgTxt("data_stream_error_general");
			break;
		case data_stream_error_invalid_message:
			mexErrMsgTxt("invalid message data");
			break;
		case data_stream_file_error:
			mexErrMsgTxt("data_stream_target_unreachable");
			break;
		case data_stream_error_busy:
			mexErrMsgTxt("data_stream_busy");
			break;
		case data_stream_wrong_stream_params:
			mexErrMsgTxt("data_stream_params_unknown");
			break;
			default: mexErrMsgTxt("data_stream_other_error");
			break;
		};

	}
	
	plhs[0] = return_value_mx;   
    plhs[1] = timestamp_mx;

}
