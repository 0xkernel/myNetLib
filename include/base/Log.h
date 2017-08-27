#ifndef __BASE_LOG_H__
#define __BASE_LOG_H__
#define MAXLINE 1024
#include <stdio.h>
void err_doit(int errnoflag,int level,const char *fmt,va_list ap);
void err_sys(const char *fmt,...);//系统调用失败时的错误消息显示
void err_msg(const char *fmt,...);//错误消息显示，不退出程序
void err_exit(const char *fmt,...);//错误消息显示，退出程序
#endif
