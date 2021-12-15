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


int main(int argc, char const *argv[])
{
    const char separator[2] = ";";
    char *token;
    int *tokenI = malloc(80 * sizeof(int)), i = 0, somme = 0, moyenne = 0; 
    int fd;

    // FIFO file path
    char * myfifo = malloc(80*sizeof(char));
    strcpy(myfifo, "/tmp/") ;
    strcat(myfifo, argv[1]);
    printf("Fichier : %s \n", myfifo);


    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80] = {"0"};

    // Open FIFO for Read only
    fd = open(myfifo, O_RDONLY);

    // Read from FIFO
    read(fd, arr1, sizeof(arr1));

    printf(" %s\n", arr1);

    // get the first token
    token = strtok(arr1, separator);

    // Print the read message
    while (token != NULL )
    {
        printf(" %s\n", token);
        somme += atoi(token);
        token = strtok(NULL, separator);
        i++;
    }


    moyenne = somme / i;
    

    printf("Moyenne: %d\n", moyenne);
    close(fd);    

    //printf("%s \n", argv[3]);


    return 0;
}
