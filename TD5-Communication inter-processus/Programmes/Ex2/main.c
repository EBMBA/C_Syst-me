#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>

struct sigaction action;
int randomNumber;
int segment_id;
char *shared_memory;

void handler(int signum)
{

    printf("-----------------------FILS-------------------- \n");
    /* Réattache le segment de mémoire partagée à une adresse différente . */
    shared_memory = (char *)shmat(segment_id, (void *)0x5000000, 0);
    printf("FILS:  mémoire partagée réattachée à l ’ adresse % p \n ", shared_memory);
    /* Affiche la chaîne de la mémoire partagée . */
    printf(" % s pid: %d\n ", shared_memory, getpid());
    /* Détache le segment de mémoire partagée . */
    shmdt(shared_memory);
}

int processChild()
{
    // Attends le signal
    while (1)
    {
        sleep(1);
    }
}

int main()
{
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x400;
    /* Alloue le segment de mémoire partagée . */
    segment_id = shmget(IPC_PRIVATE, shared_segment_size,
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    /* Attache le segment de mémoire partagée . */
    shared_memory = (char *)shmat(segment_id, 0, 0);
    printf("----------------------PARENTS------------------------\n");
    printf(" PARENTS: mémoire partagée attachée à l ’ adresse % p \n ", shared_memory);
    /* Détermine la taille du segment . */
    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf(" taille du segment : % d \n ", segment_size);

    // Fais le lien entre l'action et le signal 
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, NULL);

    // Création du fils 
    pid_t pid = fork();

    if (pid == 0) // Action à faire dans le fils
    {
        processChild();
    }

    // Initialisation du temps à 0 pour les valeurs aléatoires
    srand(time(0));

    // Boucle de création et écriture des n nombres aléatoires
    for (int i = 0; i < 5; i++)
    {
        printf("----------------------PARENTS------------------------\n");
        /* Réattache le segment de mémoire partagée à une adresse différente . */
        shared_memory = (char *)shmat(segment_id, (void *)0x5000000, 0);
        // Génération du nombre aléatoire
        randomNumber = random() % 100 + 1;
        /* Écrit une chaîne dans le segment de mémoire partagée . */
        sprintf(shared_memory, "%d", randomNumber);
        /* Détache le segment de mémoire partagée . */
        shmdt(shared_memory);
        //envoie du signale au fils
        kill(getpid(), SIGUSR1);
    }

    // Libère la mémoire 
    shmctl(segment_id, IPC_RMID, 0);
    // Arrête le processus du fils
    kill(pid, SIGINT);

    return (0);
}