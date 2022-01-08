#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile7L.bin";
    FILE *pFile;
    pFile = fopen(file, "rb");
    int read =  0; 

    fseek(pFile, 0, SEEK_SET);

    if (pFile != NULL)
    {
        for (int i = 0; i < 100; i++)
        {
            fread(&read, sizeof(int), 1, pFile);            
            printf("%d ", read);
            fseek(pFile, +1/2*sizeof(int), SEEK_CUR);
        }
        printf("\n");
        fclose(pFile);
    }
    return 0;
}