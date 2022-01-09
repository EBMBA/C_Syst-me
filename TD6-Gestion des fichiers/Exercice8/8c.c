#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile8A.bin";
    int pFile;
    pFile = open(file, O_RDONLY);
    int readVar = 0;

    lseek(pFile, 0, SEEK_SET);

    if (pFile != 0)
    {
        for (int i = 0; i < 7; i++)
        {
            read(pFile, &readVar, sizeof(int));            
            printf("%d ", readVar);
            lseek(pFile, +2*sizeof(int), SEEK_CUR);
        }

        lseek(pFile, +5*sizeof(int), SEEK_SET);
        read(pFile, &readVar, sizeof(int));
        printf("5eme valeur : %d ", readVar);
        
        printf("\n");
        close(pFile);
    }
    return 0;
}