/*************************************************************************
	> File Name: client.c
	> Author: Kenshin Wang
	> Mail: 879231132@qq.com
	> Created Time: 2015年07月15日 星期三 11时48分30秒
 ************************************************************************/

#include "head.h"

#define PORT 36000
char *ipstr = "127.0.0.1";
int main()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_exit("socket failed");
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    struct in_addr ip;
    if (inet_aton(ipstr, &ip) ==0 )
    {
        err_exit("invalid ip address");
    }
    addr.sin_addr = ip;
    int res = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (res)
    {
        err_exit("connect failed");
    }
    char buf[100];
    memset(buf, 0, sizeof(buf));
    read(sock, buf, sizeof(buf));
    printf("%s\n", buf);
}

