#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 1024
#define SA struct sockaddr

void printUsage()
{
    printf("USAGE: ./UKSU_client -h <host> -r <port> -u <username> -p <password>");
}

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the command : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Server Message\n\t%s\n", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

int login(char *loginInfo, int sockfd)
{
    char buff[MAX];
    int n, confirm = 0;
    bzero(buff, MAX);
    write(sockfd, loginInfo, sizeof(loginInfo));
    printf("Data Send it: %s\n", loginInfo);
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s\n", buff);
    printf("Kıyaslama: %d\n", strcmp(buff, "Confirmed"));
    if (strcmp(buff, "Confirmed") == 0)
    {
        confirm = 1;
        strcpy(buff, "Confirmed");
    }
    else
    {
        bzero(buff, sizeof(buff));
    }

    write(sockfd, buff, sizeof(buff));
    printf("Data Send it: %s\n", buff);
    return confirm;
}

int main(int argc, char **argv)
{
    int port = 0, port_flag = 0, host_flag = 0, username_flag = 0, password_flag = 0, option, sockfd, connfd;
    char *host = NULL, *username = NULL, *password = NULL, *loginInfo;
    struct sockaddr_in servaddr, cli;
    opterr = 0;

    while ((option = getopt(argc, argv, "r:h:u:p:")) != -1)
    {
        switch (option)
        {
        case 'r':
            port = atoi(optarg);
            port_flag = 1;
            break;
        case 'h':
            host = optarg;
            host_flag = 1;
            break;
        case 'u':
            username = optarg;
            username_flag = 1;
            break;
        case 'p':
            password = optarg;
            password_flag = 1;
            break;
        case '?':
            if (optopt == 'r' || optopt == 'h' || optopt == 'u' || optopt == 'p')
            {
                fprintf(stderr, "The -%c option requires an argument.\n", optopt);
                printUsage();
            }
            else if (isprint(optopt))
            {
                fprintf(stderr, "The option -%c is unknown.\n", optopt);
                printUsage();
            }
            else
            {
                fprintf(stderr, "The option character '\\ x%x 'is unknown.\n", optopt);
                printUsage();
            }
            return 1;
        default:
            abort();
        }
    }
    if (port_flag == 1 && host_flag == 1 && username_flag == 1 && password_flag == 1)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            printf("socket creation failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(host);
        servaddr.sin_port = htons(port);

        // connect the client socket to server socket
        if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
        {
            printf("connection with the server failed...\n");
            exit(0);
        }
        else
            printf("connected to the server..\n");
        char *u1 = "u:";
        char *u2 = "p:";
        loginInfo = (char *)malloc(7 + strlen(username) + strlen(password));
        strcpy(loginInfo,u1);
        strcat(loginInfo, username);
        strcat(loginInfo,u2);
        strcat(loginInfo, password);
        if (login(loginInfo, sockfd) == 1)
        {
            printf("Login ");
            func(sockfd);
        }
        else
        {
            printf("Login Failed");
            close(sockfd);
        }
    }
    else
    {
        printUsage();
    }
    return 0;
}
