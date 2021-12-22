#include "../lib/common.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9000
#define LINE_SIZE 1024

void send_data(int sockfd);

int main()
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = SERV_PORT;
    inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr);
    int connect_res = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_res < 0)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    char sent_line[LINE_SIZE], recv_line[LINE_SIZE + 1];

    struct sockaddr *reply_addr;
    reply_addr = malloc(sizeof(server_addr));
    socklen_t len;
    while (fgets(sent_line, LINE_SIZE, stdin) != NULL)
    {
        int i = strlen(sent_line);
        if (sent_line[i - 1] == '\n')
        {
            sent_line[i - 1] = 0;
        }
        printf("now senting %s \n", sent_line);
        int rt = sendto(sockfd, sent_line, strlen(sent_line), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (rt < 0)
        {
            perror("sent error");
        }

        printf("sent success size %d \n", rt);
        len = 0;
        recv_line[0] = 0;
        int recv = recvfrom(sockfd, recv_line, strlen(recv_line), 0, reply_addr, &len);
        if(recv < 0){
            perror("recive error");
        }
        fputs(recv_line,stdout);
        fputs("\n",stdout);
    }
}
