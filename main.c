/* system example : DIR */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */

int executeer (char *command)
{
  int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  printf ("Executing command ls...\n");
  i=system (command);
  printf ("The value returned was: %d.\n",i);
  return 0;
}

int main(){
    while (1==1)
    {
        char command[20];

        printf("Hello. Give me a command\n");
        scanf("%s", &command);
        executeer(command);
    }
    
}