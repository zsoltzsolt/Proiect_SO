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
#include <ctype.h>
#include <inttypes.h>

#define MAX_PROCESS_NUM 255

void verifyInputArguments(int argc, char **argv){

    if(argc != 4){ // We need 4 arguments (program name, input directory, output directory, character)
        perror("Invalid number of arguments!");
        exit(-1);
    }

    if(isDirectory(argv[1]) == 0){ // Input directory must exist and must be a directory
        perror("First argument is not a directory");
        exit(-1);
    }

    if(directoryExists(argv[2]) == 0) // If output directory does not exist, create it
        createDirectory(argv[2]);

    if((strlen(argv[3]) != 1) || (isalnum(argv[3][0]) == 0)){ // Third argument must be a single alphanumeric character
        perror("Third argument is not a alphanumeric character");
        exit(-1);
    }

}

void createPipe(int *pipefd){
    if(pipe(pipefd) < 0){
        perror("Error creating pipe");
        exit(-1);
    }
}

void closeReadEnd(int *pipefd){
    if(close(pipefd[0]) < 0){
        perror("Error closing pipe");
        exit(-1);
    }
}

void closeWriteEnd(int *pipefd){
    if(close(pipefd[1]) < 0){
        perror("Error closing pipe");
        exit(-1);
    }
}

void bmpHandler(char *path, char *outputPath){
    pid_t pid;

    if((pid = fork()) < 0){
        perror("Error\n");
            exit(1);
    }
    else if(pid == 0){
        transformToGrayscale(path);
        exit(0);
    }

    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        int exit_value = getFileStatistics(path, outputPath, 1);
        exit(exit_value);
    }
}

int fileHandler(char *path, char *outputPath, char *c){
    pid_t pid;
    // Daca e fisier, creez 2 pipeuri (1 pentru comunicare intre cei doi fii si unul de la fiul la parinte)
    int ff[2];
    int fp[2];

    createPipe(ff);
    createPipe(fp);

    // Primul fiu va scrie fisierul de statistica
    // Totodata, va scrie in pipe-ul ff continutul fisierului
    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        closeReadEnd(fp);
        closeWriteEnd(fp);
        closeReadEnd(ff);

        int numberOfLines = getFileStatistics(path, outputPath, 0);
        char numberOfLinesString[10];
        sprintf(numberOfLinesString, "%d", numberOfLines);

        dup2(ff[1], 1);
        closeWriteEnd(ff);

        execlp("bash", "bash","scripts/lines.sh", path, numberOfLinesString, NULL);
        perror("Error executing cat\n");
        exit(1);
    }

    // Acest proces va prii de la celalt fiu prin pipe continutul fisierului si va numara propozitiile corecte
    // Numarul de propozitii corecte va fi scris in pipe-ul fp - transmis parintelui
    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        closeWriteEnd(ff);
        closeReadEnd(fp);

        dup2(ff[0], 0);
        dup2(fp[1], 1);

        closeReadEnd(ff); closeWriteEnd(fp);

        execlp("bash", "bash", "scripts/script.sh", c, NULL);
        perror("Error executing script\n");
        exit(1);
    }

    // Procesul parinte
    closeReadEnd(ff); closeWriteEnd(ff); closeWriteEnd(fp);

    char value[10];
    if (read(fp[0], &value, 10) < 0){
        perror("Error reading from pipe");
        exit(-1);
    }
    closeReadEnd(fp);

    return strtol(value, NULL, 10);
}

void linkHandler(char *path, char *outputPath){
    pid_t pid;
    int exit_value = 0;

    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        exit_value = getLinkStatistics(path, outputPath);
        exit(exit_value);
    }
}

void directoryHandler(char *path, char *outputPath){
    pid_t pid;
    int exit_value = 0;

    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        exit_value = getDirectoryStatistics(path, outputPath);
        exit(exit_value);
    }
}

void waitProcesses(int numOfCreatedProcesses){
    int status;
    for(int j = 0; j < numOfCreatedProcesses; ++j){
        int wpid = wait(&status);
        if(WIFEXITED(status))
            printf("S-a incheiat procesul cu PID-ul %d si codul %d\n", wpid, WEXITSTATUS(status));
    }
}

void scanDirectory(char *inputDirectory, char *outputDirectory, char *c){

    struct dirent *directoryContent;
    DIR *directory = openDirectory(inputDirectory);
    int numOfCorrectSentences = 0;
    int numOfCreatedProcesses = 0;

    readdir(directory); readdir(directory); // Skip . and ..

    while((directoryContent = readdir(directory)) != NULL){
        char path[255], outputPath[255];
        sprintf(outputPath, "%s/%s_%s",outputDirectory, directoryContent->d_name, "statistica.txt"); // Construct output path
        sprintf(path, "%s/%s", inputDirectory, directoryContent->d_name); // Construct current file path

        // If it is BMP file create 2 separate processes and exit with code 10 (num lines)
        if(isBMPFile(path)){
            numOfCreatedProcesses += 2;
            bmpHandler(path, outputPath);
        }
        else if(isFile(path)){
            numOfCreatedProcesses += 2;
            numOfCorrectSentences += fileHandler(path, outputPath, c);
        }
        // Else for other files create a single process
        else if(isLink(path)){
            numOfCreatedProcesses += 1;
            linkHandler(path, outputPath);
        }

        else if(isDirectory(path)){
            numOfCreatedProcesses += 1;
            directoryHandler(path, outputPath);
        }    
    }
    // Wait for all processes to end and print PID and exit code
    waitProcesses(numOfCreatedProcesses);

    printf("Au fost identificate in total %d propozitii corecte care contin caracterul %c\n", numOfCorrectSentences, c[0]);

    closeDirectory(directory);
}


int main(int argc, char **argv){

    verifyInputArguments(argc, argv);
    
    scanDirectory(argv[1], argv[2], argv[3]);

    return 0;
}