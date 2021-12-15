#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int childProcess()
{
    while (1)
    {
        sleep(1);
    }
    // exit(0);
}

void handler(int signum)
{
    printf(" Hello from PID %d\n", (int)getpid());
}

int main(int argc, char *argv[])
{

    /* fork a child process */
    struct sigaction action;
    pid_t pid[5] = {0};
    int i = 0;
    int nbrFils = 5;
    action.sa_handler = handler;

    sigaction(SIGUSR1, &action, NULL);

    for (i = 0; i < nbrFils; i++)
    {
        pid[i] = fork();
        srand(time(0));
        if (pid[i] < 0)
        {
            fprintf(stderr, "Fork Failed");
            return 1;
        }

        else if (pid[i] == 0)
        {
            childProcess();
        }

    }
    for (int n = 0; n < 30; n++)
    {
        int num = rand() % 5;
        kill(pid[num], SIGUSR1);
        sleep(1);
    }
    for (int p = 0; p < 4; p++)
    {
        kill(pid[p], SIGINT);
    }
    
}
