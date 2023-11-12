#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "../include/directory.h"
#include "../include/file.h"


DIR *openDirectory(char *path){
    
    DIR *directory;

    if((directory = opendir(path)) == NULL){
        perror("Error opening directory!");
        exit(-1);
    }

    return directory;

}

void closeDirectory(DIR *directory){

    if(closedir(directory)){
        perror("Error while closing directory");
        exit(-1);
    }

}

int isDirectory(char *path){

    struct stat fileStat;

    if(lstat(path, &fileStat)){
        perror("Error fetching file stat");
        exit(-1);
    }

    return S_ISDIR(fileStat.st_mode);

}

void printDirectoryStatistics(directoryData data, int outputFile){

    char string[255];

    sprintf(string, "nume director: %s\n", data.directoryName);
    write(outputFile, string, strlen(string));

    sprintf(string, "identificatorul utilizatorului: %d\n", data.uid);
    write(outputFile, string, strlen(string));

    sprintf(string, "drepturi de access user: %s\n", data.rights.userRights);
    write(outputFile, string, strlen(string));

    sprintf(string, "drepturi de access grup: %s\n", data.rights.groupRights);
    write(outputFile, string, strlen(string));

    sprintf(string, "drepturi de access altii: %s\n", data.rights.othersRights);
    write(outputFile, string, strlen(string));


}

void getDirectoryStatistics(char *directoryPath, int outputFile){

    struct stat fileStat;
    directoryData data;

    lstat(directoryPath, &fileStat);

    strcpy(data.directoryName, directoryPath);

    data.uid = getUserID(directoryPath);

    strcpy(data.rights.userRights, getUserRights(directoryPath));

    strcpy(data.rights.groupRights, getGroupRights(directoryPath));

    strcpy(data.rights.othersRights, getOtherRights(directoryPath));

    printDirectoryStatistics(data, outputFile);


}
