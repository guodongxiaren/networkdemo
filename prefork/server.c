/*************************************************************************
	> File Name: server.c
	> Author: Kenshin Wang
	> Mail: 879231132@qq.com
	> Created Time: 2015年07月14日 星期二 16时36分09秒
 ************************************************************************/

#include "head.h"

#define COUNT 10
#define PORT  36000

void do_something(pid_t spid, int connfd)
{
    char buf[50];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "您好,%d号进程为您服务", spid);
    printf("%s\n", buf);
    write(connfd, buf, strlen(buf));
}
void child_main(int i, int listenfd, int addrlen)
{
    int connfd;
    socklen_t clilen;
    struct sockaddr *cliaddr;

    cliaddr = malloc(addrlen);
    printf("child %d starting...\n", getpid());
    for (;;)
    {
        sleep(3);
        clilen = addrlen;
        connfd = accept(listenfd, cliaddr, &clilen);
        if (connfd == -1)
        {
            err_exit("accept failed");
        }
        do_something(getpid(), connfd);
        close(connfd);
    }
}
pid_t child_make(int i, int listenfd, int addrlen)
{
    pid_t pid;
    if ( (pid = fork()) >0 )
        return pid;
    child_main(i, listenfd, addrlen);
}

int main()
{
    pid_t pids[COUNT];
    int sock, i;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <0 )
    {
        err_exit("socket _failed");
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        err_exit("bind failed");
    }
    if (listen(sock, 5) < 0)
    {
        err_exit("listen failed");
    }

    for (i = 0; i < COUNT; i++)
    {
        child_make(i, sock, sizeof(addr));
    }
}
