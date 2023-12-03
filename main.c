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

#define MAX_PROCESS_NUM 255

void verifyInputArguments(int argc, char **argv){

    if(argc != 4){
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

    if((strlen(argv[3]) != 1) || (isalnum(argv[3][0]) == 0)){
        perror("Third argument is not a alphanumeric character");
        exit(-1);
    }

}

void scanDirectory(char *inputDirectory, char *outputDirectory, char *c){

    struct dirent *directoryContent;

    DIR *directory = openDirectory(inputDirectory);
    char newLine[] = "\n\n";
    pid_t pids[MAX_PROCESS_NUM], wpid;
    int status, i = 0;

    readdir(directory);
    readdir(directory);

    while((directoryContent = readdir(directory)) != NULL){
        char path[255], fileName[255];
        sprintf(fileName, "%s/%s_%s",outputDirectory, directoryContent->d_name, "statistica.txt");
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
            ++i;
            if((pids[i] = fork()) < 0){
                perror("Error\n");
                exit(1);
            }
            else if(pids[i] == 0){
                getFileStatistics(path, outputFile, 1);
                exit(10);
            }
            
        }
        else if(isFile(path)){

            int ff[2];
            int fp[2];

            if (pipe(ff) < 0) {
                perror("Eroare creeare pipe!");
                exit(1);
            }

            if (pipe(fp) < 0) {
                perror("Eroare creeare pipe!");
                exit(1);
            }

            if((pids[i] = fork()) < 0){
                perror("Error\n");
                exit(1);
            }
            else if(pids[i] == 0){
                
                close(fp[0]);
                close(fp[1]);
                close(ff[0]);

                getFileStatistics(path, outputFile, 0);

                dup2(ff[1], 1);

                close(ff[1]);

                execlp("cat", "cat", path, NULL);

                perror("Eroare exec cat");
                exit(1);
            }
            ++i;
            if((pids[i] = fork()) < 0){
                perror("Error\n");
                exit(1);
            }
            else if(pids[i] == 0){
                close(ff[1]);
                close(fp[0]);

                dup2(ff[0], 0);
                dup2(fp[1], 1);

                if(close(ff[0]) < 0) {
                    perror("Eroare inchidere capat pipe!");
                    exit(1);
                }
                if(close(fp[1]) < 0) {
                    perror("Eroare inchidere capat pipe!");
                    exit(1);
                }

                    execlp("bash", "bash", "scripts/script.sh", c, NULL);
            }

            // Procesul parinte
            close(ff[0]);
            close(ff[1]);
            close(fp[1]);

            // Așteaptă ca procesul copil B să se termine
            waitpid(pids[i], NULL, 0);

            char buffer[100];
    
            while (read(fp[0], buffer, sizeof(buffer)) > 0)
            {
                printf("Sunt %c propozitii corecte\n", buffer[0]);
            }
    
    
            close(fp[0]);
            
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

    scanDirectory(argv[1], argv[2], argv[3]);

    return 0;
}