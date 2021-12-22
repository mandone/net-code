#include "../lib/common.h"

#define SERV_PORT 9000
#define LINE_SIZE 1024
#define LS_LINE_SIZE 10240
#define SERV_ADDR "127.0.0.1"
int main()
{

    int listenfd, confd;
    struct sockaddr_in client_addr, server_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_port = SERV_PORT;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    inet_pton(AF_INET, SERV_ADDR, &server_addr.sin_addr);
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    int bindrs = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindrs < 0)
    {
        perror("bind error");
    }

    int listenrs = listen(listenfd, 1024);
    if (listenrs < 0)
    {
        perror("listen error");
    }
    socklen_t client_len = sizeof(client_addr);

    char buff[LINE_SIZE];
    while (1)
    {
        confd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
        if (confd < 0)
        {
            perror("conect error");
        }

        while (1)
        {
            bzero(buff, sizeof(buff));
            int readrs = read(confd, buff, sizeof(buff));
            printf("read result %d\n", readrs);

            if (readrs < 0)
            {
                printf("read error \n");
                perror("read error");
            }
            else if (readrs == 0)
            {
                printf("read closed \n");
                close(confd);
                break;
            }

            buff[readrs] = 0;
            printf("buff content:%s \n", buff);
            if (strncmp(buff, "pwd", readrs) == 0)
            {
                char buff[LINE_SIZE];
                char *res = getcwd(buff, LINE_SIZE);
                printf("pwd result:%s \n", buff);
                int sendrs = write(confd, res, strlen(res));
                printf("write result: %d \n", sendrs);
                if (sendrs < 0)
                {
                    return 1;
                }
            }
            else if (strncmp(buff, "cd ", 3) == 0)
            {
                printf("enter to execute cd\n");
                char target[LINE_SIZE];
                bzero(target, sizeof(target));
                memcpy(target, buff + 3, strlen(buff) - 3);
                printf("target dir:%s\n", target);
                int cdrs = chdir(target);
                printf("change dir result:%d \n", cdrs);
                if (cdrs < 0)
                {
                    printf("cd error");
                }
                char *cdres = "change dir result:" + cdrs;
                if (write(confd, cdres, strlen(cdres)) < 0)
                {
                    return 1;
                }
            }
            else if (strncmp(buff, "ls ", 3) == 0)
            {
                printf("enter to execute ls\n");
                char target[LS_LINE_SIZE];
                bzero(target, sizeof(target));
                memcpy(target, buff + 3, strlen(buff) - 3);
                printf("target dir:%s\n", target);
                DIR *dir;
                struct dirent *ptr;
                dir = opendir(target);
                char buff[LINE_SIZE];
                while ((ptr = readdir(dir)) != NULL)
                {
                    if(strcmp(".",ptr->d_name) == 0 || strcmp("..",ptr->d_name) == 0){
                        continue;
                    }
                    sprintf(buff, "%s\n", ptr->d_name);
                    if (write(confd, buff, strlen(buff)) < 0)
                    {
                        return 1;
                    }
                }

                closedir(dir);
            }
            else
            {
                char *errorInfo = "unknown cmd\n";
                if (write(confd, errorInfo, strlen(errorInfo)) < 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
