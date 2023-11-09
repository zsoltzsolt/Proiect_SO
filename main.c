#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

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

int endsWithBMP(char *name){

    char extension[5];
    int lenght = strlen(name);

    if(lenght < 4)
        return 0;

    for(int i = 0 ; i < 4; ++i)
        extension[i] = name[lenght - 4 + i];

    extension[4] = '\0';


    return !strcmp(extension, ".bmp");
 

}

int isBMPFile(char *filePath){

    int fileDescriptor = openFile(filePath);
    struct stat fileStat;

    fstat(fileDescriptor, &fileStat);

    close(fileDescriptor);

    if(S_ISREG(fileStat.st_mode) && endsWithBMP(filePath))
        return 1;

    return 0;

}

void verifyInputArguments(int argc, char **argv){

    if(argc != 2){
        perror("Invalid number of arguments!");
        exit(-1);
    }

    if(isBMPFile(argv[1]) == 0){
        perror("Given argument is not a BMP file");
        exit(-1);
    }

}



typedef struct imageData{
    char imageName[255];
    int height;
    int width;
    int size;
    int links;
}imageData;

void writeImageStatistics(imageData data){
    
    printf("nume fisier: %s\n", data.imageName);
    printf("inaltime: %d\n", data.height);
    printf("latime: %d\n", data.width);
    printf("dimensiune: %d octeti\n", data.size);
    printf("numar legaturi: %d\n", data.links);

}

int getImageHeight(int imageDescriptor){

    int height;

    lseek(imageDescriptor, 22, SEEK_SET);

    if((read(imageDescriptor, &height, 4)) != 4){
        perror("Error reading BMP image height");
        exit(-1);
    }

    return height;

}

int getImageWidth(int imageDescriptor){

    int width;

    lseek(imageDescriptor, 18, SEEK_SET);

    if((read(imageDescriptor, &width, 4)) != 4){
        perror("Error reading BMP image width");
        exit(-1);
    }

    return width;

}

int getImageSize(int imageDescriptor){

    int size;

    lseek(imageDescriptor, 2, SEEK_SET);

    if((read(imageDescriptor, &size, 4)) != 4){
        perror("Error reading BMP image width");
        exit(-1);
    }

    return size;

}

int getUserID(int imageDescriptor){

    int uid;
    struct stat imageData;

    fstat(imageDescriptor, &imageData);

    uid = imageData.st_uid;

    return uid;

}


int getNumberOfLinks(int imageDescriptor){

    int numberOfLinks;
    struct stat imageData;

    fstat(imageDescriptor, &imageData);

    numberOfLinks = imageData.st_nlink;

    return numberOfLinks;

}


void getImageStatistics(char *imagePath){

    imageData data;
    int image = openFile(imagePath);

    strcpy(data.imageName, imagePath);

    data.height = getImageHeight(image);

    data.width = getImageWidth(image);
    
    data.size = getImageSize(image);

    data.links = getNumberOfLinks(image);

    writeImageStatistics(data);

    closeFile(image);

}



int main(int argc, char **argv){


    verifyInputArguments(argc, argv);

    getImageStatistics(argv[1]);

    return 0;
}