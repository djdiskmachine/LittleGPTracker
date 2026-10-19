#include "Tiny2NosStub.h"
#include "System/Console/Trace.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void fprintf(I_File *f,char *fmt,...) {
     char buffer[256] ;
     va_list args;
     va_start(args,fmt);
     int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
     va_end(args);
     if (len<0) return ;
     if (len<(int)sizeof(buffer)) {
         f->Write(buffer,1,len) ;
         return ;
     }
     // Longer than the buffer (an attribute can be, like a plugin's
     // state): print it again on the heap rather than past the stack
     char *big=(char *)malloc(len+1) ;
     if (!big) return ;
     va_start(args,fmt);
     vsnprintf(big,len+1,fmt,args) ;
     va_end(args);
     f->Write(big,1,len) ;
     free(big) ;
} ;
