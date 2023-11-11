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
#include "./include/imageStatistics.h"

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


int main(int argc, char **argv){


    verifyInputArguments(argc, argv);

    int outputFile = createFile("./output/statistici.txt");

    getImageStatistics(argv[1], outputFile);

    return 0;
}