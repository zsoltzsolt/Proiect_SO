#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bmp.h"
#include "../include/file.h"
#include<math.h>
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 1200

int endsWithBMP(char *name){
    int lenght = strlen(name);
    // If length is smaller than 4 it can't be a valid bmp file (min 4 chars .bmp)
    if(lenght < 4)
        return 0;
    //  Check if last 4 chars are .bmp   
    return strncmp(name + lenght - 4, ".bmp", 4) == 0;
}

int isBMPFile(char *filePath){
    struct stat fileStat = getFileStat(filePath);
    // To be a valid bmp file it must be a regular file and it must end with .bmp
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

void modifyColorTable(int imageDescriptor, uint16_t bitsCount){
    int n, red, green, blue, new_value, count = 0;
    char buffer[4];
    // Move our cursor at the begining of ColorTable
    lseek(imageDescriptor, 54, SEEK_SET);
    // Color table contains 2^bitsCount entries
    while(count < pow(2,bitsCount) && (n = read(imageDescriptor, buffer, 4)) > 0){
        lseek(imageDescriptor, -n, SEEK_CUR); // Move cursor back so we can overwrite RGB values
        
        count += 1; // Count how many entries we have read
        red = buffer[0]; green = buffer[1]; blue = buffer[2]; // Get RGB values

        new_value = red * .299f + green * .587f + blue * .144f; // Calculate new value

        buffer[0] = buffer[1] = buffer[2] = new_value;
        
        write(imageDescriptor, buffer, 4);// Overwrite RGB values
    }
}

void modifyRasterData(int imageDescriptor, int dataOffset){
    int n, red, green, blue, new_value;
    char buffer[BUFFER_SIZE];
    // Move our cursor at the begining of RasterData
    lseek(imageDescriptor, dataOffset, SEEK_SET);
    // Read a chunk of RGB values (3 for each pixel)
    while((n = read(imageDescriptor, buffer, BUFFER_SIZE)) > 0){
        // Move cursor back where we read current values in buffer
        lseek(imageDescriptor, -n, SEEK_CUR);
        // For each pixel we have 3 values so we work on 3 bytes at each iteration
        for(int i = 0; i + 2 < n; i += 3){
            red = buffer[i]; green = buffer[i+1]; blue = buffer[i+2];

            new_value = red * .299f + green * .587f + blue * .144f;

            buffer[i] = buffer[i+1] = buffer[i+2] = new_value;
        }
        // Overwrite RGB values
        write(imageDescriptor, buffer, n);
    }
}

void transformToGrayscale(char *imagePath){
    int imageDescriptor, dataOffset;
    uint16_t bitsCount; // I use uint16_t because bitsCount is 2 bytes long

    imageDescriptor = openFile(imagePath);
    // Bytes 10-13 represents the offset where the pixels start
    lseek(imageDescriptor, 10, SEEK_SET);

    if(read(imageDescriptor, &dataOffset, 4) != 4){
        perror("Failed to get dataOffset");
        exit(-1);
    }
    // Bytes 28-29 represents the number of bits per pixel
    lseek(imageDescriptor, 28, SEEK_SET);

    if(read(imageDescriptor, &bitsCount, 2) != 2){
        perror("Failed to get bitsCount");
        exit(-1);
    }
    /* When dealing with bmp files we have 2 cases:
        1. If bitsCount <= 8 we have a color table and we need to modify it
        2. If bitsCount > 8 we don't have a color table and we need to modify raster data
    */
    if(bitsCount <= 8)
        modifyColorTable(imageDescriptor, bitsCount);
    else
        modifyRasterData(imageDescriptor, dataOffset);

    closeFile(imageDescriptor);
}

