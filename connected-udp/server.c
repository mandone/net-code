#include "../lib/common.h"

#define SERV_PORT 9000
#define SERV_IP "127.0.0.1"
#define LINE_SIZE 1024
#define MAX_SIZE 4096
#define END_TEXT "Bye!"

static int count;
static void recv_count(int signo)
{
    printf("\nreceived udp request counts:%d \n", count);
    exit(0);
}

/*tcp server*/
int main()
{
    int cfd;
    //socket
    cfd = socket(AF_INET, SOCK_DGRAM, 0);
    //bind
    struct sockaddr_in client_addr, server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = SERV_PORT;
    inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr);
    int bind_res = bind(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_res < 0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, recv_count);
    socklen_t client_len = 0;
    char message[LINE_SIZE];
    count = 0;
    client_len = sizeof(client_addr);
    //receive
    int rec = recvfrom(cfd, message, LINE_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
    if (rec < 0)
    {
        perror("receive failed");
    }
    message[rec] = 0;
    printf("received meaasge %d bytes:%s\n", rec, message);
    //connect
    int con = connect(cfd, (struct sockaddr *)&client_addr, client_len);
    if (con < 0)
    {
        perror("failed connect");
    }

    //进入while循环
    while (strncmp(message, END_TEXT, strlen(END_TEXT)) != 0)
    {
        char send_line[MAX_SIZE];
        sprintf(send_line, "Hi %s", message);
        //send
        ssize_t sendContet = send(cfd, send_line, strlen(send_line), 0);
        if (sendContet < 0)
        {
            perror("send error");
        }

        printf("sent size %zu \n", sendContet);
        //重置meaasge数据
        ssize_t recContent = recv(cfd, message, LINE_SIZE, 0);
        if (recContent < 0)
        {
            perror("receive error ");
        }
        count++;
    }

    exit(0);
}
