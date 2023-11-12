#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../include/links.h"

int isLink(char *path){

    struct stat fileStat;

    if(lstat(path, &fileStat)){
        perror("Error fetching file stat");
        exit(-1);
    }

    if(S_ISLNK(fileStat.st_mode))
        return 1;
    return 0;

}

void printLinkStatistics(linkData data, int outputFile){
    
    char buffer[255];

    sprintf(buffer, "nume legatura: %s\n", data.linkName);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "dimensiune legatura: %d\n", data.linkSize);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "dimensiune fisier: %d\n", data.targetSize);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces user: %s\n", data.rights.userRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces grup: %s\n", data.rights.groupRights);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "drepturi de accces altii: %s\n", data.rights.othersRights);
    write(outputFile, buffer, strlen(buffer));

}

void getLinkStatistics(char *linkPath, int outputFile){

    struct stat linkStat;
    struct stat targetStat;
    linkData data;

    lstat(linkPath, &linkStat);
    stat(linkPath, &targetStat);

    strcpy(data.linkName, linkPath);

    data.linkSize = linkStat.st_size;

    data.targetSize = targetStat.st_size;

    strcpy(data.rights.userRights, getUserRights(linkPath));

    strcpy(data.rights.groupRights, getGroupRights(linkPath));

    strcpy(data.rights.othersRights, getOtherRights(linkPath));

    printLinkStatistics(data, outputFile);


}


