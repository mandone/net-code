#include "../lib/common.h"

#define SERV_PORT 9000

void read_data(int sockfd);


/*tcp server*/
int main()
{
    int listenfd, cfd;
    socklen_t length;
    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr,sizeof(server_addr));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_res = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_res < 0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    listen(listenfd, 1024);

    while (1)
    {
        /* code */
        length = sizeof(client_addr);
        cfd = accept(listenfd, (struct sockaddr *)&client_addr, &length);
        read_data(cfd);
        close(cfd);
    }

    return 0;
}

void read_data(int sockfd)
{
    int time = 0;
    char buf[1024];
    ssize_t n;

    for (;;)
    {
        fprintf(stdout, "block in read \n");
        n = read(sockfd, buf, 1024);
        if (n == 0)
        {
            return;
        }
        if (n < 0)
        {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}