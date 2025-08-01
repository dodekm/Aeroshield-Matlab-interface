
%% Step response

COM_port=2;
baudrate=250000;
data_stream_ptr=data_stream_start_mex(COM_port,baudrate);
clear t
clear y

Ts=0.05;

u0=30;

%[~,~]=data_stream_read_mex(data_stream_ptr,1,1);
for i=1:100
    [y(i),t(i)]=data_stream_read_mex(data_stream_ptr,1,1);
    data_stream_write_mex(data_stream_ptr,1,single(u0));
end
data_stream_write_mex(data_stream_ptr,1,single(0));
data_stream_end_mex(data_stream_ptr);

plot(t,y,'-k','LineWidth',1.5)
grid on
xlabel('t[s]')
ylabel('phi[deg]')





