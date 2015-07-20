/*************************************************************************
	> File Name: head.h
	> Author: Kenshin Wang
	> Mail: 879231132@qq.com
	> Created Time: 2015年07月15日 星期三 11时24分24秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void err_exit(char *str)
{
    perror(str);
    exit(1);
}
#endif
