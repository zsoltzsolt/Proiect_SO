#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "imageStatistics.h"
#include "bmp.h"

void writeImageStatistics(imageData data, int outputFile){
    
    char buffer[255];

    sprintf(buffer, "nume fisier: %s\n", data.imageName);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "inaltime: %d\n", data.height);
    write(outputFile, buffer, strlen(buffer));

    sprintf(buffer, "latime: %d\n", data.width);
    write(outputFile, buffer, strlen(buffer));

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



void getImageStatistics(char *imagePath, int outputFile){

    imageData data;
    int image = openFile(imagePath);

    strcpy(data.imageName, imagePath);

    data.height = getImageHeight(image);

    data.width = getImageWidth(image);
    
    data.size = getImageSize(image);

    data.links = getNumberOfLinks(image);

    strcpy(data.imageRights.userRights, getUserRights(image));

    strcpy(data.imageRights.groupRights, getGroupRights(image));

    strcpy(data.imageRights.othersRights, getOtherRights(image));

    data.uid = getUserID(image);

    data.date = getModificationDate(image);

    writeImageStatistics(data, outputFile);

    closeFile(image);

}