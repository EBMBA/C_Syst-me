#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

void childProcess(int p[2], char inbuf[1])
{
    int somme = 0;
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            /* read pipe */
            read(p[0], inbuf, 1);
            printf("%s\n", inbuf);
            somme = somme + atoi(inbuf);
        }
        printf("Somme : %d\n", somme);
    }
}

/* Renvoie un nombre aléatoire compris dans l’intervalle [ low , high ].*/
int random_range(unsigned const low, unsigned const high)
{
    unsigned const range = high - low + 1;
    return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0));
}

int main()
{
    char *inbuf = malloc(sizeof(int));
    int p[2], valeur, n;
    pid_t pid;

    if (pipe(p) < 0)
        exit(1);

    pid = fork();

    if (pid == 0)
    {
        childProcess(p, inbuf);
    }

    /* continued */
    /* write pipe */
    // Initialisation du temps à 0 pour les valeurs aléatoires
    srand(time(0));
    n = random_range(5, 20);

    for (int i = 0; i < n; i++)
    {
        sprintf(inbuf, "%d", random_range(0, 9));
        write(p[1], inbuf, 1);
    }

    // Arrête le processus du fils
    wait(NULL);

    return 0;
}