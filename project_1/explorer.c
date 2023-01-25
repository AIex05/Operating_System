#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

void Random(int upper, int lower, int *pointer)
{
    *pointer = (rand() % (upper - lower + 1)) + lower;
}

const char * array[] = {"/home", "/proc", "/proc/sys", "/usr", "/usr/bin", "/bin"};

int main()
{
    char seed[10000];
    int Random_number[5] = {rand(), rand(), rand(), rand(), rand()};
    int seed_int, counter;
    FILE *fp = fopen("seed.txt", "r");
    while (fscanf(fp, "%s", seed) != EOF)
    {
        printf("The seed is %s\n", seed);
    }
    fclose(fp);
    seed_int = atoi(seed);
    srand(seed_int);
    printf("It's time for a big world tour\n");

    int PID = 1;
    char *cmd = "ls";
    char *ary[3] = {"ls", "-tr", NULL};

    for (counter = 0; counter < 5; counter++)
    {
        if (PID != 0)
        {
            chdir(array[Random_number[counter]%6]);
            char buf[1024];
            getcwd(buf, 1024);
            printf("The directory is \n    %s\n", buf);
            PID = fork();
        }
        if (PID == 0)
        {
            int my_pid = getpid();
            printf("[Child, PID: %d]:Executing 'ls -tr' cmd\n", my_pid);
            execvp(cmd, ary);
        }
        if (PID != 0)
        {
            int status;
			int my_pid = getpid();
			printf("[Parent, PID: %d]: Waiting for %d\n", my_pid, PID);
			waitpid(PID, &status, 0);
        }
    }
    exit(0);
}