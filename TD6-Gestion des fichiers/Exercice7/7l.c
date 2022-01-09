#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile7L.bin";
    FILE *pFile;
    pFile = fopen(file, "wb+");

    if (pFile != NULL)
    {
        for (int i = 0; i < 100; i++)
        {
            fwrite(&i, sizeof(i), 1, pFile);            
            printf("%d", i);
        }
        fclose(pFile);
    }
    return 0;
}