#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

struct job {
int id ;
int length ;
// other meta data
struct job * next ;
};


int main(int argc, char **argv){

    char *policy;
    char *Infile_location;
    struct job Jobs[argc - 3];

    for (int i = 0; i < argc; i++) {
        if (i == 1) {
            policy = (char*) argv[i];
        }
        if (i == 2) {
            Infile_location = (char*) argv[i];
        }
        if (i > 2) {
            Jobs[i-3].id = i-3;
            Jobs[i-3].length = (int) *argv[i];
        }
    }

    //File Opener code: https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int ct = 0;

    fp = fopen(Infile_location, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:", read);
        printf("%d\n", line);
        Jobs[ct].id = ct;
        Jobs[ct].length = (int) line;
    }

    fclose(fp);
    if (line)
        free(line);
    
    printf("Policy: %s\n",policy);
    printf("Jobs[0].id: %d\n",Jobs[0].id);
    printf("Jobs[0].length: %d\n",Jobs[0].length);
}