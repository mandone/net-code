#include "/lib/common.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9527

int main(void)
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ]; 

    /*����һ��socket ָ��IPv4 TCP*/
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    /*��ʼ��һ����ַ�ṹ:*/
    bzero(&serv_addr, sizeof(serv_addr));                       //����
    serv_addr.sin_family = AF_INET;                             //IPv4Э����
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    //ָ��IP �ַ�������ת��Ϊ�����ֽ��� ��3:��������
    serv_addr.sin_port = htons(SERV_PORT);                      //ָ���˿� ����ת�����ֽ���

    /*���ݵ�ַ�ṹ����ָ������������*/
    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        /*�ӱ�׼�����ȡ����*/
        fgets(buf, sizeof(buf), stdin);
        /*������д��������*/
        write(sfd, buf, strlen(buf));       //д��������
        /*�ӷ���������ת��������*/
        len = read(sfd, buf, sizeof(buf));
        /*д����׼���*/
        write(STDOUT_FILENO, buf, len);
    }

    /*�ر�����*/
    close(sfd);

    return 0;
}