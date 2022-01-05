
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

int getSizeDirectory(char path[100], struct stat stats );
int getFileSize(struct stat stats);

int main()
{
    char path[100];
    struct stat stats;

    printf("Enter source file path: ");
    scanf("%s", path);


    // stat() returns 0 on successful operation,
    // otherwise returns -1 if unable to get file properties.
    if (stat(path, &stats) == 0)
    {
        printf("Size of the repertory : %d\n", getSizeDirectory(path, stats));
    }
    else
    {
        printf("Unable to get file properties.\n");
        printf("Please check whether '%s' file exists.\n", path);
    }

    return 0;
}

int getSizeDirectory(char path[100], struct stat stats ){
    int size = 0; 
    DIR *dp;
    struct dirent *dirp;
    char pathFile[200];

    printf("path : %s\n", path);
    dp = opendir(path);

    while ((dirp = readdir(dp)) != NULL){
        strcpy(pathFile, path);
        strcat(pathFile, "/");
        strcat(pathFile, dirp->d_name);
        stat(pathFile, &stats);
        size += getFileSize(stats); 
        //printf("size: %d name: %s\n", size, pathFile );
    } 

    closedir(dp);

    return size;
}

int getFileSize(struct stat stats)
{
    // File size
    int size =  512 * stats.st_blocks;
    //printf("%d\n", size );
    return  size; // size of an block * numbers of block 
}