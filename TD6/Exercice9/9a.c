#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>

typedef struct
{
    char nom[20];
    int age;
    float taille;
} Personne;

void saveInFile(Personne *personnes)
{
    char *file = "BinaryFile9C.bin";
    FILE *pFile;
    pFile = fopen(file, "wb");
    if (pFile != NULL)
    {
        fwrite(&personnes, sizeof(personnes), 1, pFile);

        printf("\n");

        fclose(pFile);
    }
}

void readInFile()
{
    char *file = "BinaryFile9C.bin";
    FILE *pFile;
    pFile = fopen(file, "rb");
    Personne readVar[4];

    if (pFile != 0)
    {
        fread(readVar, sizeof(readVar), 1, pFile);
        for (int i = 0; i < 4; i++)
        {
            printf("nom : %s, age: %d,taille : %.2f\n", readVar[i].nom, readVar[i].age, readVar[i].taille);
        }
        fclose(pFile);
    }
}

int main(int argc, char const *argv[])
{
    Personne tab[4];

    strcpy(tab[0].nom, "eleve1");
    tab[0].age = 15;
    tab[0].taille = 1.98;

    strcpy(tab[1].nom, "eleve2");
    tab[1].age = 16;
    tab[1].taille = 1.58;

    strcpy(tab[2].nom, "eleve3");
    tab[2].age = 15;
    tab[2].taille = 1.65;

    strcpy(tab[3].nom, "eleve4");
    tab[3].age = 14;
    tab[3].taille = 1.69;

    for (int i = 0; i < 4; i++)
    {
        printf("nom : %s, age: %d,taille : %.2f\n", tab[i].nom, tab[i].age, tab[i].taille);
    }

    // char *file = "BinaryFile9C.bin";
    // FILE *pFile;
    // pFile = fopen(file, "wb");
    // if (pFile != NULL)
    // {
    //     fwrite(tab, sizeof(tab), 1, pFile);
    //     fclose(pFile);
    // }

    // pFile = fopen(file, "rb");
    // Personne readVar[4];

    // if (pFile != 0)
    // {
    //     fread(readVar, sizeof(readVar), 1, pFile);
    //     for (int i = 0; i < 4; i++)
    //     {
    //         printf("nom : %s, age: %d,taille : %.2f\n", readVar[i].nom, readVar[i].age, readVar[i].taille);
    //     }
    //     fclose(pFile);
    // }

    saveInFile(tab);
    readInFile();

    /*
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
    */
    return 0;
}