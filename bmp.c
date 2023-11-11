#include "bmp.h"
#include "file.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

    close(fileDescriptor);

    if(S_ISREG(fileStat.st_mode) && endsWithBMP(filePath))
        return 1;

    return 0;

}

int getImageHeight(int imageDescriptor){

    int height;

    lseek(imageDescriptor, 22, SEEK_SET);

    if((read(imageDescriptor, &height, 4)) != 4){
        perror("Error reading BMP image height");
        exit(-1);
    }

    return height;

}

int getImageWidth(int imageDescriptor){

    int width;

    lseek(imageDescriptor, 18, SEEK_SET);

    if((read(imageDescriptor, &width, 4)) != 4){
        perror("Error reading BMP image width");
        exit(-1);
    }

    return width;

}

int getImageSize(int imageDescriptor){

    int size;

    lseek(imageDescriptor, 2, SEEK_SET);

    if((read(imageDescriptor, &size, 4)) != 4){
        perror("Error reading BMP image width");
        exit(-1);
    }

    return size;

}