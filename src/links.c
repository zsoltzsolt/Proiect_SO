#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../include/links.h"
#include "../include/file.h"

int isLink(char *path){

    struct stat fileStat = getFileStat(path);

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

    // linkStat coresponds to metadata about LINK
    linkStat = getFileStat(linkPath);
    
    // targetStat cresponds to metadata about the ACTUAL file
    if(stat(linkPath, &targetStat)){
        perror("Error fetching file stat");
        exit(-1);
    }

    strcpy(data.linkName, linkPath);

    data.linkSize = linkStat.st_size;

    data.targetSize = targetStat.st_size;

    strcpy(data.rights.userRights, getUserRights(linkStat));

    strcpy(data.rights.groupRights, getGroupRights(linkStat));

    strcpy(data.rights.othersRights, getOtherRights(linkStat));

    printLinkStatistics(data, outputFile);

}


