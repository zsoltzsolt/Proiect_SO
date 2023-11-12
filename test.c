#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>

int main(int argc, char **argv){

    struct stat statistics;

    lstat(argv[1], &statistics);

    printf("%d", S_ISLNK(statistics.st_mode));

    return 0;
}