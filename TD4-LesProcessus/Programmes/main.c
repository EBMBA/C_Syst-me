#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

extern int globalValue;

void bye(){
    printf("bye\n");
}

int main(int argc, char *argv[])
{

    /* fork a child process */
    pid_t pid;
    int i = 0;
    int nbrFils = 0;
    int globalValue = 0;

    printf("Number of childs: ");
    scanf("%d", &nbrFils);

    while (1)
    {
        if (i < nbrFils)
        {
            for (int i = 0; i < nbrFils; i++)
            {
                pid = fork();
                srand(time(0));
                if (pid < 0)
                {
                    fprintf(stderr, "Fork Failed");
                    return 1;
                }

                else if (pid == 0)
                {
                    int num = (rand() % (12 - 10 + 1)) + 10;
                    sleep(num);
                    printf("I'm the child. Number : %d. PID : %d G : %d\n", i, (int)getpid(), globalValue);
                    globalValue = (rand() % (120 - 10 + 1)) + 10;
                    printf("I'm the child. Number : %d. PID : %d G : %d\n", i, (int)getpid(), globalValue);
                    atexit(bye);
                    execl("/home/emile/Bureau/ICS_RandomGenerator/dist/Debug/GNU-Linux/ics_randomgenerator" , "ics_randomgenerator" , "-n", "15", NULL);
                    exit(0);
                }

                else
                {
                    wait(NULL);
                    printf("Child Complete and global value G : %d\n", globalValue);
                }
            }
        }
    }

}