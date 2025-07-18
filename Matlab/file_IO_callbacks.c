#include "file_IO_callbacks.h"
#include <windows.h>
#include <stdio.h>


size_t file_read_callback(void* handle,void* buff, size_t btr,uint32_t timeout) {
      
   HANDLE hSerial=(HANDLE)handle; 
   DWORD dwBytesRead = 0;
   if(!ReadFile(hSerial, (char*)buff , btr, &dwBytesRead, NULL)){
        return 0;
   }
   return  (size_t)dwBytesRead;
        
}
size_t file_write_callback(void* handle,const void* buff, size_t btw,uint32_t timeout) {
   
   HANDLE hSerial=(HANDLE)handle; 
   DWORD dwBytesWrite = 0;
   if(!WriteFile(hSerial, (const char*)buff , btw, &dwBytesWrite, NULL)){
        return 0;
   }
   if (!FlushFileBuffers(hSerial)) {
    return 0;
   }

   return  (size_t)dwBytesWrite;
    
}

