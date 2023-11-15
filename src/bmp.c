#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bmp.h"
#include "../include/file.h"

int endsWithBMP(char *name){

    char extension[5];
    int lenght = strlen(name);

    // If length is smaller than 4 it can't be a valid bmp file (min 4 chars .bmp)
    if(lenght < 4)
        return 0;

    // We extract last 4 characters
    for(int i = 0 ; i < 4; ++i)
        extension[i] = name[lenght - 4 + i];

    extension[4] = '\0';

    return !strcmp(extension, ".bmp");
 
}

int isBMPFile(char *filePath){

    struct stat fileStat = getFileStat(filePath);

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

int getPixel(int imageDescriptor){
    
    int value;

    if((read(imageDescriptor, &value, 1)) != 1){
            perror("Error reading pixel value");
            exit(-1);
        }

    return value;

}

void transformToGrayscale(char *imagePath){

    int imageDescriptor = openFile(imagePath);
    int red, green, blue;
    int height;
    int width;

    height = getImageHeight(imageDescriptor);
    width = getImageWidth(imageDescriptor);
    
    lseek(imageDescriptor, 54, SEEK_SET);
    int i;
    for(i = 0; i < height*width; ++i){
        red = getPixel(imageDescriptor);
        green = getPixel(imageDescriptor);
        blue = getPixel(imageDescriptor);

        int new_v = 0.3*red + 0.6*green + 0.1*blue;

        lseek(imageDescriptor, -3, SEEK_CUR);

        write(imageDescriptor, &new_v, 1);
        write(imageDescriptor, &new_v, 1);
        write(imageDescriptor, &new_v, 1);

    }

    closeFile(imageDescriptor);

}

