#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "../include/directory.h"
#include "../include/file.h"

#define BUFFER_SIZE 1024


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
        perror("Error fetching file stat3");
        exit(-1);
    }

    return S_ISDIR(fileStat.st_mode);

}

void printDirectoryStatistics(fileData data, int outputFile){

    char string[BUFFER_SIZE];

    sprintf(string, "nume director: %s\n"
                "identificatorul utilizatorului: %d\n"
                "drepturi de access user: %s\n"
                "drepturi de access grup: %s\n"
                "drepturi de access altii: %s\n",
        data.name, data.uid, data.rights.userRights, data.rights.groupRights, data.rights.othersRights);

    write(outputFile, string, strlen(string));

}

int getDirectoryStatistics(char *directoryPath, int outputFile){

    struct stat fileStat;
    fileData data;

    fileStat = getFileStat(directoryPath);

    strcpy(data.name, directoryPath);

    data.uid = fileStat.st_uid;

    strcpy(data.rights.userRights, getUserRights(fileStat));

    strcpy(data.rights.groupRights, getGroupRights(fileStat));

    strcpy(data.rights.othersRights, getOtherRights(fileStat));

    printDirectoryStatistics(data, outputFile);

    return 5;
}
