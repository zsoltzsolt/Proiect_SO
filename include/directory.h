#ifndef __DIRECTORY_H
#define __DIRECTORY_H


DIR *openDirectory(char *path);

void closeDirectory(DIR *directory);

int directoryExists(char *path);

void createDirectory(char *path);

int isDirectory(char *path);

int getDirectoryStatistics(char *directoryPath, char *outputFile);


#endif