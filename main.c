#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int openFile(char *filePath){

    int file = 0;

    if((file = open(filePath, O_RDONLY)) < 0){
        perror("Error while opening file");
        exit(-1);
    }

    return file;

}

int endsWithBMP(char *name){

    char extension[5];
    int lenght = strlen(name);

    if(lenght < 4)
        return 0;

    for(int i = 0 ; i < 4; ++i)
        extension[i] = name[lenght - 4 + i];

    extension[4] = '\0';


    return !strcmp(extension, ".bmp");
 

}

int isBMPFile(char *filePath){

    int fileDescriptor = openFile(filePath);
    struct stat fileStat;

    fstat(fileDescriptor, &fileStat);

    if(S_ISREG(fileStat.st_mode) && endsWithBMP(filePath))
        return 1;

    return 0;

}

void verifyInputArguments(int argc, char **argv){

    if(argc != 2){
        perror("Invalid number of arguments!");
        exit(-1);
    }

    if(isBMPFile(argv[1]) == 0){
        perror("Given argument is not a BMP file");
        exit(-1);
    }

}

int main(int argc, char **argv){


    verifyInputArguments(argc, argv);

    return 0;
}