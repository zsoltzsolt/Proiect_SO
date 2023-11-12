#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "../include/imageStatistics.h"
#include "../include/bmp.h"

void writeImageStatistics(imageData data, int outputFile, int isImage){
    
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

    writeImageStatistics(data, outputFile, isImage);

    closeFile(image);

}




