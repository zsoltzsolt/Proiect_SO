#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "../include/file.h"
#include "../include/bmp.h"

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

int isFile(char *filePath){

    struct stat fileStat;

    lstat(filePath, &fileStat);

    if(S_ISREG(fileStat.st_mode))
        return 1;

    return 0;

}

int getUserID(char *path){

    int uid;
    struct stat imageData;

    lstat(path, &imageData);

    uid = imageData.st_uid;

    return uid;

}

int getSize(char *path){

    int size;
    struct stat imageData;

    lstat(path, &imageData);

    size = imageData.st_size;

    return size;

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

void writeFileStatistics(imageData data, int outputFile, int isImage){
    
    char buffer[255];

    sprintf(buffer, "nume fisier: %s\n", data.imageName);
    write(outputFile, buffer, strlen(buffer));

    if(isImage){
        sprintf(buffer, "inaltime: %d\n", data.height);
        write(outputFile, buffer, strlen(buffer));

        sprintf(buffer, "latime: %d\n", data.width);
        write(outputFile, buffer, strlen(buffer));
    }

    sprintf(buffer, "dimensiune: %d octeti\n", data.size);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "numar legaturi: %d\n", data.links);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "identificatorul utilizatorului: %d\n", data.uid);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "timul ultimei modificari: %s\n", data.date);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces user: %s\n", data.imageRights.userRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces grup: %s\n", data.imageRights.groupRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces altii: %s\n", data.imageRights.othersRights);
    write(outputFile, buffer, strlen(buffer));

}



void getFileStatistics(char *imagePath, int outputFile, int isImage){

    imageData data;
    int image = openFile(imagePath);

    strcpy(data.imageName, imagePath);
    
    data.size = getSize(imagePath);

    if(isImage){
        data.height = getImageHeight(image);
        data.width = getImageWidth(image);
    }

    data.links = getNumberOfLinks(imagePath);

    strcpy(data.imageRights.userRights, getUserRights(imagePath));

    strcpy(data.imageRights.groupRights, getGroupRights(imagePath));

    strcpy(data.imageRights.othersRights, getOtherRights(imagePath));

    data.uid = getUserID(imagePath);

    data.date = getModificationDate(imagePath);

    writeFileStatistics(data, outputFile, isImage);

    closeFile(image);

}

