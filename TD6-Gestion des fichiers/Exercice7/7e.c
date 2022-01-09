#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile7E.bin";
    FILE *pFile;
    pFile = fopen(file, "w");
    long int ByteArray = 0x4142434451525354L;
    printf("Hexadecimal : %lx \n Decimal : %ld \n",ByteArray,ByteArray);
    if (pFile != NULL)
    {
        fprintf(pFile, "%lo", ByteArray);

        fclose(pFile);
    }
    return 0;
}
