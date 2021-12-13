#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int reader(char *file) {
    int fd;
    void * file_memory;
    int integer[5];

    /* Ouvre le fichier */
    fd = open(file, O_RDWR, S_IRUSR | S_IWUSR);
    /* Met en co rr es pon da nce le fichier et la mémoire */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Lit l’entier , l’affiche  */
    sscanf(file_memory, " %d %d %d %d %d ", &integer[0],&integer[1], &integer[2],&integer[3],&integer[4]);
    for (int i = 0; i < 5; i++)
    {
        printf(" valeur : %d \n ", integer[i]);
    }
    
    
    //sprintf((char *) file_memory, " %d \n ", integer);
    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return 0;
}

int main(int argc, char const *argv[])
{
    reader("./test.txt");
    return 0;
}
