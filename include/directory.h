#ifndef __DIRECTORY_H
#define __DIRECTORY_H

#include "file.h"

typedef struct directoryData{
    char directoryName[255];
    int uid;
    rights rights;
}directoryData;

DIR *openDirectory(char *path);

void closeDirectory(DIR *directory);

int isDirectory(char *path);

int getDirectoryStatistics(char *directoryPath, int outputFile);


#endif