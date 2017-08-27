#include "Log.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void err_doit(int errnoflag,int level,const char *fmt,va_list ap){
	int errno_save,n;
	char buf[MAXLINE+1] = {0};

	errno_save = errno;
	vsnprintf(buf,MAXLINE,fmt,ap);
	n = strlen(buf);

	if(errnoflag){
		snprintf(buf+n,MAXLINE - n,":%s",strerror(errno_save));
	}
	strcat(buf,"\n");
	
	fputs(buf,stderr);
	fflush(stderr);
}

void err_sys(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(1,0,fmt,ap);
	va_end(ap);
	exit(1);
}

void err_msg(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(0,0,fmt,ap);
	va_end(ap);
}

void err_exit(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(0,0,fmt,ap);
	va_end(ap);
}
