#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "executer.h"

#define MAX 1024
#define SA struct sockaddr

void printUsage()
{
    printf("USAGE: ./UKSU_server -r <port> -u <username> -p <password>");
}

// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
	char *result_t;
	int result;
    for (;;)
    {
        bzero(buff, MAX);

		read(sockfd, buff, sizeof(buff));
		result = executeShellCommand(buff);

		printf("Client Request: %s\n", buff);
		bzero(buff, MAX);
		if (result == 0)
		{
			result_t = "Execute is completed\n";
		}
		else if (result == -1)
		{
			result_t = "*** ERROR: forking child process failed\n";
		}
		else
		{
			result_t = "*** ERROR: exec failed\n";
		}

		write(sockfd, result_t, MAX);

		if (strncmp("exit", buff, 4) == 0)
		{
			printf("Server Exit...\n");
			break;
		}
    }
}

int login(char *loginInfo, int sockfd)
{
    char buff[MAX];
    int n, confirm = 0;
        bzero(buff, MAX);
    read(sockfd, buff, sizeof(buff));
    printf("Client: %s\nServer: %s\n",buff,loginInfo);
    if ((strcmp(buff, loginInfo)) == 0)
    {
        strcpy(buff,"Confirmed");
    } else {
        strcpy(buff,"Failed");
    }   
    write(sockfd, buff, sizeof(buff));
    printf("Data Send it %s\n", buff);
    bzero(buff, MAX);
    read(sockfd, buff, sizeof(buff));
    printf("Data Read it %s\n", buff);
    if (strcmp(buff,"Confirmed") == 0)
    {
        confirm = 1;
    }
    printf("Confirm: %d\n",confirm);
    fflush(stdin);
    return confirm;
}

int main(int argc, char **argv)
{
    int port = 0, port_flag = 0, username_flag = 0, password_flag = 0, sockfd, connfd, len, option;
    char *username = NULL, *password = NULL, *loginInfo;
    struct sockaddr_in servaddr, cli;

    opterr = 0;

    while ((option = getopt(argc, argv, "r:u:p:")) != -1)
    {
        switch (option)
        {
        case 'r':
            port = atoi(optarg);
            port_flag = 1;
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
            if (optopt == 'r' || optopt == 'u' || optopt == 'p')
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
    if (port_flag == 1 && username_flag == 1 && password_flag == 1)
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
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);

        // Binding newly created socket to given IP and verification
        if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
        {
            printf("socket bind failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully binded..\n");

        // Now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0)
        {
            printf("Listen failed...\n");
            exit(0);
        }
        else
            printf("Server listening..\n");
        len = sizeof(cli);

        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
            printf("server acccept the client...\n");

        char *u1 = "u:";
        char *u2 = "p:";
        loginInfo = (char *)malloc(7 + strlen(username) + strlen(password));
        strcpy(loginInfo,u1);
        strcat(loginInfo, username);
        strcat(loginInfo,u2);
        strcat(loginInfo, password);
        
        if (login(loginInfo, connfd) == 1)
        {
            printf("Login\n");
            fflush(stdin);
            func(connfd);
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
