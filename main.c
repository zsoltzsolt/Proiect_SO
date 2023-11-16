#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "./include/bmp.h"
#include "./include/file.h"
#include "./include/directory.h"
#include "./include/links.h"
#include <dirent.h>


void verifyInputArguments(int argc, char **argv){

    if(argc != 2){
        perror("Invalid number of arguments!");
        exit(-1);
    }

    if(isDirectory(argv[1]) == 0){
        perror("Given argument is not a directory");
        exit(-1);
    }

}

void scanDirectory(char *directoryPath, int outputFile){

    struct dirent *directoryContent;

    DIR *directory = openDirectory(directoryPath);
    char newLine[] = "\n\n";

    readdir(directory);
    readdir(directory);

    while((directoryContent = readdir(directory)) != NULL){
        char path[255];
        sprintf(path, "%s/%s", directoryPath, directoryContent->d_name);
        if(isLink(path)){
            getLinkStatistics(path, outputFile);
            write(outputFile, newLine, 2);
        }
        else if(isBMPFile(path)){
            printf("here");
            transformToGrayscale(path);
            getFileStatistics(path, outputFile, 1);
            write(outputFile, newLine, 2);
        }
        else if(isFile(path)){
            getFileStatistics(path, outputFile, 0);
            write(outputFile, newLine, 2);
        }
        else if(isDirectory(path)){
            getDirectoryStatistics(path, outputFile);
            write(outputFile, newLine, 2);
        }
    }

    closeDirectory(directory);

}


int main(int argc, char **argv){


    verifyInputArguments(argc, argv);
    
    int outputFile = createFile("./output/statistici.txt");

    scanDirectory(argv[1], outputFile);

    return 0;
}