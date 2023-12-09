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
        perror("Error while closing directory!");
        exit(-1);
    }
}

int isDirectory(char *path){
    struct stat fileStat;

    if(lstat(path, &fileStat)){
        perror("Error fetching directory stat");
        exit(-1);
    }

    return S_ISDIR(fileStat.st_mode);
}

void printDirectoryStatistics(fileData data, char *outputPath){
    char string[BUFFER_SIZE];
    int outputFile = createFile(outputPath);

    sprintf(string, "nume director: %s\n"
                "identificatorul utilizatorului: %d\n"
                "drepturi de access user: %s\n"
                "drepturi de access grup: %s\n"
                "drepturi de access altii: %s\n",
        data.name, data.uid, data.rights.userRights, data.rights.groupRights, data.rights.othersRights);
    write(outputFile, string, strlen(string));

    closeFile(outputFile);
}

int getDirectoryStatistics(char *directoryPath, char *outputPath){
    struct stat fileStat;
    fileData data;

    fileStat = getFileStat(directoryPath);

    strcpy(data.name, getFileNameFromPath(directoryPath));

    data.uid = fileStat.st_uid;

    strcpy(data.rights.userRights, getUserRights(fileStat));
    strcpy(data.rights.groupRights, getGroupRights(fileStat));
    strcpy(data.rights.othersRights, getOtherRights(fileStat));

    printDirectoryStatistics(data, outputPath);

    return 5; // Number of lines printed
}
