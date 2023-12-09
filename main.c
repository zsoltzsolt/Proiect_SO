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

    if(isDirectory(argv[2]) == 0){ // Output directory must exist and must be a directory
        perror("First argument is not a directory");
        exit(-1);
    }

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

void bmpHandler(char *path, char *outputPath){
    pid_t pid;

    if((pid = fork()) < 0){ // First child will transform the image to grayscale
        perror("Error\n");
            exit(1);
    }
    else if(pid == 0){
        transformToGrayscale(path); // Transform image to grayscale
        exit(0); // Exit with code 0 (success)
    }

    if((pid = fork()) < 0){ // Second child will write the statistics
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        int numberOfLines = getFileStatistics(path, outputPath, 1); // Write statistics in file (1 means bmp file, 0 means other file)
        exit(numberOfLines);  // exit with number of lines written in statistics file
    }
}

int fileHandler(char *path, char *outputPath, char *c){
    pid_t pid;
    int ff[2], fp[2];
    // We have 2 pipes: ff - for sending file content, fp - for sending number of correct sentences
    createPipe(ff);
    createPipe(fp);
    // First child will create statistics file and write the content of the initial file in pipe ff
    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        close(ff[0]); close(fp[0]); close(fp[1]);

        int numberOfLines = getFileStatistics(path, outputPath, 0);
        char numberOfLinesString[10];
        sprintf(numberOfLinesString, "%d", numberOfLines); // Convert number of lines to string to be passed as argument

        dup2(ff[1], 1); // Redirect stdout to pipe ff
        close(ff[1]);

        execlp("bash", "bash", "scripts/lines.sh", path, numberOfLinesString, NULL); // Script to write file content in pipe ff and also exit this process with number of lines written
        perror("Error executing cat\n");
        exit(1);
    }

    // Second child will get the content of the file from pipe ff and will count the number of correct sentences
    // It will also write the number of correct sentences in pipe fp
    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        close(ff[1]); close(fp[0]);

        dup2(ff[0], 0); // Redirect stdin to pipe ff
        dup2(fp[1], 1); // Redirect stdout to pipe fp

        close(ff[0]); close(fp[1]);

        execlp("bash", "bash", "scripts/script.sh", c, NULL);// Script to count correct sentences
        perror("Error executing script\n");
        exit(1);
    }

    close(ff[0]); close(ff[1]); close(fp[1]);

    char value[10];
    if (read(fp[0], &value, 10) < 0){   // Read number of correct sentences from pipe fp
        perror("Error reading from pipe");
        exit(-1);
    }
    close(fp[0]);

    return strtol(value, NULL, 10); // Return number of correct sentences in current file, converted from string to int
}

void linkHandler(char *path, char *outputPath){
    pid_t pid;
    int numberOfLines = 0;

    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        numberOfLines = getLinkStatistics(path, outputPath); // Write link statistics in file
        exit(numberOfLines); // Exit with number of lines written in statistics file
    }
}

void directoryHandler(char *path, char *outputPath){
    pid_t pid;
    int numberOfLines = 0;

    if((pid = fork()) < 0){
        perror("Error\n");
        exit(1);
    }
    else if(pid == 0){
        numberOfLines = getDirectoryStatistics(path, outputPath); // Write directory statistics in file
        exit(numberOfLines); // Exit with number of lines written in statistics file
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
    int numOfCorrectSentences = 0, numOfCreatedProcesses = 0;
    char path[255], outputPath[255];

    readdir(directory); readdir(directory); // Skip . and ..

    while((directoryContent = readdir(directory)) != NULL){
        sprintf(outputPath, "%s/%s_%s",outputDirectory, directoryContent->d_name, "statistica.txt"); // Construct statistics output file path
        sprintf(path, "%s/%s", inputDirectory, directoryContent->d_name); // Construct current file path

        if(isBMPFile(path)){
            numOfCreatedProcesses += 2; // In case of bmp file we create 2 processes
            bmpHandler(path, outputPath);
        }
        else if(isFile(path)){
            numOfCreatedProcesses += 2; // In case of other regular files we create 2 processes
            numOfCorrectSentences += fileHandler(path, outputPath, c); // We add the number of correct sentences from current file
        }
        else if(isLink(path)){
            numOfCreatedProcesses += 1; // In case of link we create 1 process
            linkHandler(path, outputPath);
        }

        else if(isDirectory(path)){
            numOfCreatedProcesses += 1; // In case of directory we create 1 process
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