/*************************************************************************
	> File Name: server.c
	> Author: Kenshin Wang
	> Mail: 879231132@qq.com
	> Created Time: 2015年08月16日 星期 15时06分09秒
 ************************************************************************/

#include "head.h"

#define COUNT 2
#define TCOUNT 5
#define PORT  36000

typedef struct
{
    int listenfd;
    int addrlen;
}sarg;

void do_something(pid_t pid, pthread_t tid, int connfd)
{
    char buf[100];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "您好,%lu号线程为您服务", tid);
    printf("%s\n", buf);
    write(connfd, buf, strlen(buf));
}
void * child_main(void *args)
{
    sarg * arg = (void *)args;
    int listenfd = arg->listenfd;
    int addrlen = arg->addrlen;
    int connfd;
    socklen_t clilen;
    struct sockaddr *cliaddr;

    cliaddr = malloc(addrlen);
    printf("thread %lu starting...\n", pthread_self());
    for (;;)
    {
        sleep(3);
        clilen = addrlen;
        connfd = accept(listenfd, cliaddr, &clilen);
        if (connfd == -1)
        {
            err_exit("accept failed");
        }
        do_something(getpid(), pthread_self(), connfd);
        close(connfd);
    }
    return NULL;
}
void child_make(int listenfd, int addrlen)
{
    pthread_t t1;
    int s;
    sarg arg;
    arg.listenfd = listenfd;
    arg.addrlen = addrlen;
    int i;
    for (i = 0; i< TCOUNT; i++)
    {
        s = pthread_create(&t1, NULL, child_main, &arg);
        if ( s != 0 )
        {
            err_exit("pthread_create failed");
        }
    }
    for (i = 0; i< TCOUNT; i++)
    {
        s = pthread_join(t1, NULL);
        if ( s != 0 )
        {
            err_exit("pthread_join");
        }
    }
}

int main()
{
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
        child_make(sock, sizeof(addr));
    }
}
