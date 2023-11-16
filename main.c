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

#define MAX_PROCESS_NUM 255

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

void scanDirectory(char *directoryPath){

    struct dirent *directoryContent;

    DIR *directory = openDirectory(directoryPath);
    char newLine[] = "\n\n";
    pid_t pids[MAX_PROCESS_NUM], wpid;
    int status;
    int i = 0;

    readdir(directory);
    readdir(directory);

    while((directoryContent = readdir(directory)) != NULL){
        char path[255];
        sprintf(path, "%s/%s", directoryPath, directoryContent->d_name);

        if((pids[i] = fork()) < 0){
            perror("Error\n");
            exit(1);
        }
        else if(pids[i] == 0){
            char fileName[255];
            sprintf(fileName, "%s_%s",directoryContent->d_name, "statistica.txt");
            int outputFile = createFile(fileName);

            if(isLink(path)){
                getLinkStatistics(path, outputFile);
                write(outputFile, newLine, 2);
            }
            else if(isBMPFile(path)){
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
            close(outputFile);
            exit(i);
        }

        ++i;
    }

    for(int j = 0; j < i; ++j){
        wpid = wait(&status);
        if(WIFEXITED(status))
            printf("S-a incheiat procesul cu PID-ul %d si codul %d\n", wpid, WEXITSTATUS(status));
    }

    closeDirectory(directory);

}


int main(int argc, char **argv){


    verifyInputArguments(argc, argv);
    

    scanDirectory(argv[1]);

    return 0;
}