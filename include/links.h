#ifndef __LINKS_H
#define __LINKS_H

#include "file.h"

typedef struct linkData{
    char linkName[255];
    int linkSize;
    int targetSize;
    rights rights;
}linkData;

int getLinkStatistics(char *linkPath, int outputFile);
int isLink(char *path);

#endif