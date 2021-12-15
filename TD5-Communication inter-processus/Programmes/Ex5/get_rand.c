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
    const char separator[2] = ";";
    char *token;
    int tokenI; 
    int fd;

    // FIFO file path
    char * myfifo = malloc(80*sizeof(char));
    strcpy(myfifo, "/tmp/") ;
    strcat(myfifo, argv[1]);
    printf("Fichier : %s \n", myfifo);


    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80], arr2[1];

    // Open FIFO for Read only
    fd = open(myfifo, O_RDONLY);

    // Read from FIFO
    read(fd, arr1, sizeof(arr1));

    // Print the read message
    printf("User2: %s\n", arr1);
    close(fd);    

    //printf("%s \n", argv[3]);


    return 0;
}
