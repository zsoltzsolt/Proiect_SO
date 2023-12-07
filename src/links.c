#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../include/links.h"
#include "../include/file.h"

#define BUFFER_SIZE 1024

int isLink(char *path){

    struct stat targetStat, fileStat;
    fileStat = getFileStat(path);
    
    // targetStat cresponds to metadata about the ACTUAL file
    if(stat(path, &targetStat)){
        perror("Error fetching file stat1");
        exit(-1);
    }

    if(S_ISLNK(fileStat.st_mode) && S_ISREG(targetStat.st_mode))
        return 1;
    return 0;
}

void printLinkStatistics(fileData data, char *outputPath){
    char buffer[BUFFER_SIZE];
    int outputFile = createFile(outputPath);

    sprintf(buffer, "nume legatura: %s\n"
                "dimensiune legatura: %d\n"
                "dimensiune fisier: %d\n"
                "drepturi de accces user: %s\n"
                "drepturi de accces grup: %s\n"
                "drepturi de accces altii: %s\n",
        data.name, data.size, data.targetSize, data.rights.userRights, data.rights.groupRights, data.rights.othersRights);

    write(outputFile, buffer, strlen(buffer));
}

int getLinkStatistics(char *linkPath, char *outputPath){
    struct stat linkStat, targetStat;
    fileData data;
    // linkStat coresponds to metadata about LINK
    linkStat = getFileStat(linkPath);
    // targetStat cresponds to metadata about the ACTUAL file
    if(stat(linkPath, &targetStat)){
        perror("Error fetching file stat1");
        exit(-1);
    }

    strcpy(data.name, linkPath);

    data.size = linkStat.st_size;

    data.targetSize = targetStat.st_size;

    strcpy(data.rights.userRights, getUserRights(linkStat));

    strcpy(data.rights.groupRights, getGroupRights(linkStat));

    strcpy(data.rights.othersRights, getOtherRights(linkStat));

    printLinkStatistics(data, outputPath);

    return 6;
}


