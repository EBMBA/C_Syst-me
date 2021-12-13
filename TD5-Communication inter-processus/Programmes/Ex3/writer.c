#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define FILE_LENGTH 0x100

/* Renvoie un nombre aléatoire compris dans l’intervalle [ low , high ].*/
int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (RAND_MAX + 1.0));
}

int writer(char *file) {

    int fd;
    void * file_memory;

    /* Initialise le générateur de nombres aléatoires */
    srand(time(NULL));

    /* Prépare un fichier suffisamment long pour contenir le nombre . */
    fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, " ", 1);
    lseek(fd, 0, SEEK_SET);
    /* Met en correspondance le fichier et la mémoire . */
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Ecrit un entier aléatoire dans la zone mise en correspondance . */
    sprintf((char *) file_memory, " %d %d %d %d %d \n ", random_range(-100, 100),random_range(-100, 100),random_range(-100, 100),random_range(-100, 100),random_range(-100, 100));
    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    writer("./test.txt");
    return 0;
}
