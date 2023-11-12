#ifndef __FILE_H
#define __FILE_H

typedef struct rights{
    char userRights[4];
    char groupRights[4];
    char othersRights[4];
}rights;

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

void writeFileStatistics(imageData data, int outputFile, int isBMP);
void getFileStatistics(char *imagePath, int outputFile, int isBMP);

int openFile(char *filePath);

int createFile(char *filePath);

void closeFile(int fileDescriptor);

int isFile(char *filePath);

int getUserID(char *path);

int getNumberOfLinks(char *path);

int getSize(char *path);

char *getModificationDate(char *path);

char *getUserRights(char *path);

char *getGroupRights(char *path);

char *getOtherRights(char *path);


#endif