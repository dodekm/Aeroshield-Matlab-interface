
clc

mex data_stream_start_mex.c ../data_stream.c file_IO_callbacks.c -I.. -R2018a
mex data_stream_end_mex.c  ../data_stream.c file_IO_callbacks.c -I.. -R2018a
mex data_stream_read_mex.c ../data_stream.c file_IO_callbacks.c -I.. -R2018a
mex data_stream_write_mex.c ../data_stream.c file_IO_callbacks.c -I.. -R2018a
