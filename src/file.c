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

    if((file = open(filePath, O_RDWR)) < 0){
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

struct stat getFileStat(char *path){

    struct stat fileStat;

    if(lstat(path, &fileStat)){
        perror("Error fetching file stat2");
        exit(-1);
    }

    return fileStat;

}


char *getModificationDate(struct stat fileStat){

    static char modificationDate[256];

    struct tm *timeNow = localtime(&fileStat.st_mtimespec.tv_sec);

    // We convert modification date to dd/mm/yyyy format
    strftime(modificationDate, 10, "%d.%m.%Y", timeNow);

    return modificationDate;

}

char *getUserRights(struct stat fileStat){

    static char rights[4];

    int mode = fileStat.st_mode;

    rights[0] = (S_IRUSR & mode) ? 'R' : '-';
    rights[1] = (S_IWUSR & mode) ? 'W' : '-';
    rights[2] = (S_IXUSR & mode) ? 'X' : '-';
    rights[3] = '\0';

    return rights;

}

char *getGroupRights(struct stat fileStat){

    static char rights[4];

    int mode = fileStat.st_mode;

    rights[0] = (S_IRGRP & mode) ? 'R' : '-';
    rights[1] = (S_IWGRP & mode) ? 'W' : '-';
    rights[2] = (S_IXGRP & mode) ? 'X' : '-';
    rights[3] = '\0';

    return rights;

}

char *getOtherRights(struct stat fileStat){

    static char rights[4];

    int mode = fileStat.st_mode;

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

    // Image height and width is displayed only for bmp images
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

    sprintf(buffer, "timpul ultimei modificari: %s\n", data.date);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces user: %s\n", data.imageRights.userRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces grup: %s\n", data.imageRights.groupRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces altii: %s\n", data.imageRights.othersRights);
    write(outputFile, buffer, strlen(buffer));

}



void getFileStatistics(char *imagePath, int outputFile, int isImage){

    struct stat fileStat = getFileStat(imagePath);
    imageData data;
    int image = openFile(imagePath);

    strcpy(data.imageName, imagePath);
    
    data.size = fileStat.st_size;

    // if it is an image we set height and width
    if(isImage){
        data.height = getImageHeight(image);
        data.width = getImageWidth(image);
    }

    data.links = fileStat.st_nlink;

    strcpy(data.imageRights.userRights, getUserRights(fileStat));

    strcpy(data.imageRights.groupRights, getGroupRights(fileStat));

    strcpy(data.imageRights.othersRights, getOtherRights(fileStat));

    data.uid = fileStat.st_uid;

    data.date = getModificationDate(fileStat);

    writeFileStatistics(data, outputFile, isImage);

    closeFile(image);

}

