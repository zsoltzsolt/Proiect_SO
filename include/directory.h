#ifndef __DIRECTORY_H
#define __DIRECTORY_H

DIR *openDirectory(char *path);

void closeDirectory(DIR *directory);

int isDirectory(char *path);

int getDirectoryStatistics(char *directoryPath, char *outputPath);

#endif