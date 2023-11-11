#ifndef __FILE_H
#define __FILE_H

typedef struct rights{
    char userRights[4];
    char groupRights[4];
    char othersRights[4];
}rights;

int openFile(char *filePath);

int createFile(char *filePath);

void closeFile(int fileDescriptor);

int getUserID(int imageDescriptor);

int getNumberOfLinks(int imageDescriptor);

char *getModificationDate(int imageDescriptor);

char *getUserRights(int imageDescriptor);

char *getGroupRights(int imageDescriptor);

char *getOtherRights(int imageDescriptor);


#endif