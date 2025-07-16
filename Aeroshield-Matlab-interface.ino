#include "data_stream.h"
#include "AeroShield.h"


data_stream_timestamp_t get_timestamp_impl(void*);
data_stream_server_instance data_stream_serial;


float Ts=0.05;

void setup() {              
  Serial.begin(250000);       
   data_stream_server_init(&data_stream_serial, data_stream_data_read_impl, data_stream_data_write_impl, serial_stream_read_fcn, serial_stream_write_fcn, (void*)&Serial, get_timestamp_impl, NULL,NULL);
  while(!AeroShield.begin(Ts)){}; 

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
}

void loop() {
  data_stream_server(&data_stream_serial); 
}



data_stream_return_t data_stream_data_read_impl(data_stream_id_t stream_id, uint8_t data_size, void* data_ptr,void* user_data) {


  switch (stream_id) {
    case 1:
      while(!AeroShield.isSampleAvailable()){}
      *((float*)data_ptr) = AeroShield.sensorReadDegreeSampled(); 
      return data_stream_ok;
    default:
      return data_stream_wrong_stream_params;
  };
}


data_stream_return_t data_stream_data_write_impl(data_stream_id_t stream_id, uint8_t data_size, const void* data_ptr,void* user_data) {
  switch (stream_id) {
    case 1:
       AeroShield.actuatorWriteSampled(*((const float*)data_ptr));
      return data_stream_ok;
    default:
      return data_stream_wrong_stream_params;

  };

}

data_stream_timestamp_t get_timestamp_impl(void* unused)
{
  return  AeroShield.getTick()*Ts;
}



size_t serial_stream_write_fcn(void* handle, const void* buffer, size_t size,uint32_t timeout)
{
  HardwareSerial* S = (HardwareSerial*)handle;
  return S->write((char*)buffer, size);
}
size_t serial_stream_read_fcn(void* handle, void* buffer, size_t size,uint32_t timeout)
{
  HardwareSerial* S = (HardwareSerial*)handle;
  return S->readBytes((char*)buffer, size);
}
