#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../include/file.h"

int createFile(char *filePath){

    int file = 0;

    if((file = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0){
        perror("Error while creating file");
        exit(-1);
    }

    return file;

}

int openFile(char *filePath){

    int file = 0;

    if((file = open(filePath, O_RDONLY)) < 0){
        perror("Error while opening file");
        exit(-1);
    }

    return file;

}

void closeFile(int fileDescriptor){

    if(close(fileDescriptor)){
        perror("Eroare inchidere fisier");
        exit(-1);
    }

}

int getUserID(char *path){

    int uid;
    struct stat imageData;

    lstat(path, &imageData);

    uid = imageData.st_uid;

    return uid;

}


int getNumberOfLinks(char *path){

    int numberOfLinks;
    struct stat imageData;

    lstat(path, &imageData);

    numberOfLinks = imageData.st_uid;

    return numberOfLinks;

}

char *getModificationDate(char *path){

    static char modificationDate[256];
    struct stat imageData;

    lstat(path, &imageData);

    struct tm *timeNow = localtime(&imageData.st_mtimespec.tv_sec);

    strftime(modificationDate, 10, "%d.%m.%Y", timeNow);

    return modificationDate;

}

char *getUserRights(char *path){

    static char rights[4];
    struct stat imageData;

    lstat(path, &imageData);

    int mode = imageData.st_mode;

    rights[0] = (S_IRUSR & mode) ? 'R' : '-';
    rights[1] = (S_IWUSR & mode) ? 'W' : '-';
    rights[2] = (S_IXUSR & mode) ? 'X' : '-';
    rights[3] = '\0';

    return rights;

}

char *getGroupRights(char *path){

    static char rights[4];
    struct stat imageData;

    lstat(path, &imageData);

    int mode = imageData.st_mode;

    rights[0] = (S_IRGRP & mode) ? 'R' : '-';
    rights[1] = (S_IWGRP & mode) ? 'W' : '-';
    rights[2] = (S_IXGRP & mode) ? 'X' : '-';
    rights[3] = '\0';

    return rights;

}

char *getOtherRights(char *path){

    static char rights[4];
    struct stat imageData;

    lstat(path, &imageData);

    int mode = imageData.st_mode;

    rights[0] = (S_IROTH & mode) ? 'R' : '-';
    rights[1] = (S_IWOTH & mode) ? 'W' : '-';
    rights[2] = (S_IXOTH & mode) ? 'X' : '-';
    rights[3] = '\0';

    return rights;

}