#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *file = "BinaryFile7C.bin";
    FILE *pFile;
    pFile = fopen(file, "w");
    long int ByteArray = 0x4142434451525354L;
    printf("Hexadecimal : |0x%.8x| \n Binary : \n",ByteArray,ByteArray);
    if (pFile != NULL)
    {
        fwrite(&ByteArray, sizeof(ByteArray), 1, pFile);

        fclose(pFile);
    }
    return 0;
}
