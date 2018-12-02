#ifndef ERROR_H
#define ERROR_H
#include <stdarg.h>


int ERROR  = 0;

void error(const char *x){
	ERROR = 1;
	  printf("\n####: %s\n",x);
	  //printf("EVERYTHING WERE IMMEDIATELY STOPPED\n");
}
void error2(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
 ERROR = 1;
    while (*fmt != '\0') {
        if (*fmt == 'd') {
            printf("%d", va_arg(args, int));
        } else if (*fmt == 'c') {
            printf("%c", va_arg(args, int));
        } else if (*fmt == 'f') {
            printf("%f", va_arg(args, double));
        }else if(*fmt == 'l'){
        	printf("%ld",va_arg(args,long) );
        }else if(*fmt=='s'){
        	printf("%s",va_arg(args,char*));
		}
        ++fmt;
    }
    printf("EVERYTHING WERE IMMEDIATELY STOPPED\n");
    
    va_end(args);
}
#endif