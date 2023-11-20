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

    if(argc != 3){
        perror("Invalid number of arguments!");
        exit(-1);
    }

    if(isDirectory(argv[1]) == 0){
        perror("First argument is not a directory");
        exit(-1);
    }

    if(isDirectory(argv[2]) == 0){
        perror("Second argument is not a directory");
        exit(-1);
    }

}

void scanDirectory(char *inputDirectory, char *outputDirectory){

    struct dirent *directoryContent;

    DIR *directory = openDirectory(inputDirectory);
    char newLine[] = "\n\n";
    pid_t pids[MAX_PROCESS_NUM], wpid;
    int status, i = 0;

    readdir(directory);
    readdir(directory);

    while((directoryContent = readdir(directory)) != NULL){
        char path[255], fileName[255];
        int outputFile = createFile(fileName), returnValue = 0;
        sprintf(path, "%s/%s", inputDirectory, directoryContent->d_name);
        
        // If it is BMP file create 2 separate processes and exit with code 10 (num lines)
        if(isBMPFile(path)){
            if((pids[i] = fork()) < 0){
                perror("Error\n");
                exit(1);
            }
            else if(pids[i] == 0){
                transformToGrayscale(path);
                exit(10);
            }
            if((pids[i] = fork()) < 0){
                perror("Error\n");
                exit(1);
            }
            else if(pids[i] == 0){
                getFileStatistics(path, outputFile, 1);
                exit(10);
            }
            
        }
        // Else for other files create a single process
        else if((pids[i] = fork()) < 0){
            perror("Error\n");
            exit(1);
        }
        else if(pids[i] == 0){

            if(isLink(path)){
                returnValue = 6;
                getLinkStatistics(path, outputFile);
                write(outputFile, newLine, 2);
            }

            else if(isFile(path)){
                returnValue = 8;
                getFileStatistics(path, outputFile, 0);
                write(outputFile, newLine, 2);
            }

            else if(isDirectory(path)){
                returnValue = 5;
                getDirectoryStatistics(path, outputFile);
                write(outputFile, newLine, 2);
            }
            close(outputFile);
            exit(returnValue);
        }
        ++i;
    }

    // Wait for all processes to end and print PID and exit code
    for(int j = 0; j < i; ++j){
        wpid = wait(&status);
        if(WIFEXITED(status))
            printf("S-a incheiat procesul cu PID-ul %d si codul %d\n", wpid, WEXITSTATUS(status));
    }

    closeDirectory(directory);
        
}


int main(int argc, char **argv){


    verifyInputArguments(argc, argv);

    scanDirectory(argv[1], argv[2]);

    return 0;
}