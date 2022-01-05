#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile7C.bin";
    FILE *pFile;
    pFile = fopen(file, "w");
    long int ByteArray = 19496893802562113L;
    if (pFile != NULL)
    {
        fwrite(&ByteArray, sizeof(ByteArray), 1, pFile);

        fclose(pFile);
    }
    return 0;
}
