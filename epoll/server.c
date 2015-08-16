#include "head.h"
#define SERVPORT 36000 /*服务器监听端口号 */
#define BACKLOG 10 /* 最大同时连接请求数 */
#define MAX_EVENTS 10

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts<0)
    {
        err_exit("fcntl(sock,GETFL)");
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        err_exit("fcntl(sock,SETFL,opts)");
    }   
}

void do_something(int connfd)
{
    char buf[50];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "nice to meet you");
    write(connfd, buf, strlen(buf)+1);
}

void do_use_fd(int client_fd)
{
    const char str[] = "God bless you!/n";
    if (send(client_fd,  str,  sizeof(str),  0) == -1)
        perror("send");
    close(client_fd);
}

int main()
{
    int sockfd;
    struct sockaddr_in my_addr; 
    struct sockaddr remote_addr;
    if ((sockfd = socket(AF_INET,  SOCK_STREAM,  0)) == -1)
    {
        err_exit("socket");
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVPORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(my_addr.sin_zero), 8);
    if (bind(sockfd,  (struct sockaddr *)&my_addr,  sizeof(struct sockaddr)) == -1)
    {
        err_exit("bind");
    }
    printf("bind %d ok\n", sockfd);
    if (listen(sockfd,  BACKLOG) == -1)
    {
        err_exit("listen");
    }
    printf("listen ok\n");
    struct epoll_event ev, events[MAX_EVENTS];
    int conn_sock, nfds, epollfd;
    epollfd = epoll_create(10);

    if (epollfd == -1)
    {
        err_exit("epoll_create");
    }

    printf("epoll_create\n");
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) 
    {
        err_exit("epoll_ctl: sockfd");
    }
    printf("epoll_ctl ok\n");
    for (;;)
    {
        printf("start epoll_wait\n");
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            err_exit("epoll_pwait");
        }
        printf("epoll_wait returns, nfds = %d\n", nfds);
        int n;
        socklen_t sin_size;
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == sockfd)
            {
                printf("sockfd:%d", sockfd);
                conn_sock = accept(sockfd,
                        &remote_addr, &sin_size);
                if (conn_sock == -1)
                {
                    err_exit("accept");
                }
                setnonblocking(conn_sock);
                do_something(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
                {
                    err_exit("epoll_ctl: conn_sock");
                }
            } 
            else 
            {
                do_use_fd(events[n].data.fd);
            }
        }
    }
}
