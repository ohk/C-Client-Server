#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include "executer.h"


#define MAX 512 
#define PORT 8081 
#define SA struct sockaddr 




/* EXECUTER */

void parse(char *line, char **argv)
{
    while (*line != '\0')
    { /* if not the end of line ....... */
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0'; /* replace white spaces with 0    */
        *argv++ = line;     /* save the argument position     */
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++; /* skip the argument until ...    */
    }
    *argv = '\0'; /* mark the end of argument list  */
}

int execute(char **argv)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    { /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        return -1;
    }
    else if (pid == 0)
    { /* for the child process:         */
        if (execvp(*argv, argv) < 0)
        { /* execute the command  */
            printf("*** ERROR: exec failed\n");
            return -2;
        }
    }
    else
    {                                /* for the parent:      */
        while (wait(&status) != pid) /* wait for completion  */
            ;
    }
    return 0;
}

int executeShellCommand(char *line)
{
    char *argv[64];
    parse(line, argv);                /*   parse the line               */
    if (strcmp(argv[0], "exit") == 0) /* is it an "exit"?     */
        exit(0);                      /*   exit if it is                */
    return execute(argv);                    /* otherwise, execute the command */
}









// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX];
    char *result_t; 
	int result; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
        result = executeShellCommand(buff);
        printf("Client Request: %s\n", buff); 
        bzero(buff, MAX);
        if(result){
            result_t = "Execute is completed\n";
        } else if ( result == -1){
            result_t = "*** ERROR: forking child process failed\n";
        } else {
            result_t = "*** ERROR: exec failed\n";
        }

		// and send that buffer to client 
		write(sockfd, result_t, MAX); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 
