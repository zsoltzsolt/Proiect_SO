#ifndef __FILE_H
#define __FILE_H

typedef struct rights{
    char userRights[4];
    char groupRights[4];
    char othersRights[4];
}rights;

typedef struct fileData{
    char name[255];
    int height;
    int width;
    int size;
    int targetSize;
    char *date;
    int uid;
    int links;
    rights rights;
}fileData;

int getFileStatistics(char *imagePath, char *outputFile, int isBMP);

int openFile(char *filePath);

int createFile(char *filePath);

void closeFile(int fileDescriptor);

int isFile(char *filePath);

char *getModificationDate(struct stat fileStat);

char *getUserRights(struct stat fileStat);

char *getGroupRights(struct stat fileStat);

char *getOtherRights(struct stat fileStat);

struct stat getFileStat(char *path);

#endif