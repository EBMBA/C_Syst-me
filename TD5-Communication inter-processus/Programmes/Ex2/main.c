#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define FILE_LENGTH 0x100

int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (100 + 1.0));
}

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
    int fd;
    void * file_memory;
    struct sigaction action;
    pid_t pid;
    int i = 0;
    int nbrFils = 5;
    action.sa_handler = handler;

    srand(time(NULL));
    int num = random_range(0, 100);

    printf(" Random number : %d\n", num);


    sigaction(SIGUSR1, &action, NULL);

    pid = fork();
    srand(time(0));
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    else if (pid == 0)
    {
        childProcess();
    }




    for (int n = 0; n < 30; n++)
    {
        int num = rand() % 4;
        kill(pid, SIGUSR1);
        sleep(1);
    }
    for (int p = 0; p < 4; p++)
    {
        kill(pid, SIGINT);
    }
}