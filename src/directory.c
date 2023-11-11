#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int isDirectory(char *path){

    struct stat fileStat;

    if(lstat(path, &fileStat)){
        perror("Error fetching file stat");
        exit(-1);
    }

    return S_ISDIR(fileStat.st_mode);

}
