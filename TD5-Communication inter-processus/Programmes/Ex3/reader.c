#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int reader(char *file) {
    int fd;
    int * file_memory;

    /* Ouvre le fichier */
    fd = open(file, O_RDWR, S_IRUSR | S_IWUSR);
    /* Met en co rr es pon da nce le fichier et la mémoire */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Lit l’entier , l’affiche  */
    for (int i = 0; i < 5; i++)
    {
        printf(" valeur : %d \n ", file_memory[i]);
    }
    
    
    //sprintf((char *) file_memory, " %d \n ", integer);
    /* Libère la mémoire ( facultatif car le programme se termine ) . */
    munmap(file_memory, FILE_LENGTH);
    return 0;
}

int main(int argc, char const *argv[])
{
    reader("./test.bin");
    return 0;
}
