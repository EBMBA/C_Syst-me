#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define FILE_LENGTH 0x100

/* Renvoie un nombre aléatoire compris dans l’intervalle [ low , high ].*/
int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char const *argv[])
{
    /* Initialise le générateur de nombres aléatoires */
    srand(time(NULL));
    FILE *fd;

    // FIFO file path
    char * myfifo = malloc(80*sizeof(char));
    strcpy(myfifo, "/tmp/") ;
    strcat(myfifo, argv[1]);
    printf("Fichier : %s \n", myfifo);


    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[1], arr2[1];

    for (size_t i = 0; i < atoi(argv[3]); i++)
    {
          // Open FIFO for write only
        fd = fopen(myfifo, "w");

        fprintf(fd, "%d;",random_range(-100, 100));

        fclose(fd);
    }
    

    //printf("%s \n", argv[3]);


    return 0;
}
