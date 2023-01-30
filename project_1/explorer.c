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
    int Random_number[13];
    int seed_int, counter;
    FILE *fp = fopen("seed.txt", "r");
    while (fscanf(fp, "%s", seed) != EOF)
    {
        printf("The seed is %s\n", seed);
    }
    fclose(fp);
    seed_int = atoi(seed);
    srand(seed_int);
    int upper = 5, lower = 0, Rand_Num, Rand_Num_cp;
    Random(upper, lower, &Rand_Num);
    printf("It's time for a big world tour\n");

    int PID = 0;
    char *cmd = "ls";
    char *ary[3] = {"ls", "-tr", NULL};

    for (counter = 0; counter < 6; counter++)
    {
        if (PID == 0)
        {
            chdir(array[Rand_Num]);
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