#include "../lib/common.h"

#define SERV_PORT 9000
#define LINE_SIZE 1024
#define SERV_ADDR "127.0.0.1"

int main()
{

    int sfd;
    struct sockaddr_in server_addr;

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_port = SERV_PORT;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERV_ADDR, &server_addr.sin_addr);;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t server_len = sizeof(server_addr);

    int conrs = connect(sfd, (struct sockaddr *)&server_addr, server_len);
    if (conrs < 0)
    {
        perror("connect error");
    }
    char sent_line[LINE_SIZE], recv_line[LINE_SIZE + 1];

    while (1)
    {
        if (fgets(sent_line, LINE_SIZE, stdin) != NULL)
        {
            int i = strlen(sent_line);
            if (sent_line[i - 1] == '\n')
            {
                sent_line[i - 1] = 0;
            }

            if (strncmp(sent_line, "quit", strlen(sent_line)) == 0)
            {
                if (shutdown(sfd, 1))
                {
                    perror("shutdown error");
                }
            }

            printf("now senting %s \n", sent_line);
            ssize_t rt = write(sfd, sent_line, strlen(sent_line));
            if (rt < 0)
            {
                printf("ent error \n");
                perror("sent error");
            }
            printf("sent success size %zu\n", rt);
        }
        int recvContent = read(sfd, recv_line, LINE_SIZE);
        printf("recvContent %d \n", recvContent);
        if (recvContent < 0)
        {
            perror("recive error");
        }

        else if (recvContent == 0)
        {
            printf("read closed \n");
            break;
        }
        recv_line[recvContent] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
        memset(recv_line,0,LINE_SIZE);
    }
    exit(0);
}
