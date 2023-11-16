#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bmp.h"
#include "../include/file.h"

#define BUFFER_SIZE 1200

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

void transformToGrayscale(char *imagePath){

    int imageDescriptor = openFile(imagePath);
    int red, green, blue;
    char buffer[BUFFER_SIZE];
    int n;
    int new_value;
    int dataOffset;

    // Bytes 10-13 represents the offset where the pixels start
    lseek(imageDescriptor, 10, SEEK_SET);

    if(read(imageDescriptor, &dataOffset, 4) != 4){
        perror("Failed to get dataOffset");
        exit(-1);
    }
    
    // Move our cursor at the begining of RasterData
    lseek(imageDescriptor, dataOffset, SEEK_SET);
    
    // Read a chunk of RGB values (3 for each pixel)
    while((n = read(imageDescriptor, buffer, BUFFER_SIZE)) > 0){
        // Move cursor back where we read current values in buffer
        lseek(imageDescriptor, -n, SEEK_CUR);

        // For each pixel we have 3 values so we work on 3 bytes at each iteration
        for(int i = 0; i + 2 < n; i += 3){
            red = buffer[i];
            green = buffer[i+1];
            blue = buffer[i+2];

            new_value = red * .299f + green * .587f + blue * .144f;

            buffer[i] = new_value;
            buffer[i+1] = new_value;
            buffer[i+2] = new_value;

        }
        // Overwrite RGB values
        write(imageDescriptor, buffer, n);

    }

    closeFile(imageDescriptor);

}

