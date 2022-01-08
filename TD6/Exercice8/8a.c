#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile8A.bin";
    FILE *pFile;
    pFile = fopen(file, "wb");

    if (pFile != NULL)
    {
        for (int i = 10; i <= 30; i++)
        {
            fwrite(&i, sizeof(i), 1, pFile);            
            printf("%d", i);
        }
        printf("\n");
        fclose(pFile);
    }
    return 0;
}