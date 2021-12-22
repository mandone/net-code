#include "../lib/common.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9000
#define MESSAGE_SIZE 102400

void send_data(int sockfd);

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr);
    int connect_res = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_res < 0)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    send_data(sockfd);
    exit(0);
}

void send_data(int sockfd)
{
    char *query;
    query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *copy;
    copy = query;
    size_t remain = strlen(query);
    while (remain)
    {
        long int n_writtern = send(sockfd, copy, remain, 0);
        fprintf(stdout, "send into buffer %ld \n", n_writtern);
        if (n_writtern <= 0)
        {
            perror("send failed");
            return;
        }
        remain -= n_writtern;
        copy += remain;
    }
    return;
}
