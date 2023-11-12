#ifndef __IMAGESTATISTICS_H
#define __IMAGESTATISTICS_H
#include "file.h"

typedef struct imageData{
    char imageName[255];
    int height;
    int width;
    int size;
    char *date;
    int uid;
    int links;
    rights imageRights;
}imageData;

void writeImageStatistics(imageData data, int outputFile, int isBMP);
void getFileStatistics(char *imagePath, int outputFile, int isBMP);


#endif