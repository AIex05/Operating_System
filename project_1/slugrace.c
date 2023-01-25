#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

void Random(int upper, int lower, int *pointer)
{
	*pointer = (rand() % (upper - lower + 1)) + lower;
}

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
	int upper = 12, lower = 5, Rand_Num, Rand_Num_cp;
	Random(upper, lower, &Rand_Num);
	printf("Number of processes are %d\n", Rand_Num);
    char *cmd = "./slug";
    char *ary[3];

	int PID = 1;
    for (int counter = 1; counter < 5; counter++) {
        if (PID != 0) {
            PID = fork();
        }
        if (PID == 0) {
            char ct_string[4];
            sprintf(ct_string, "%d", counter);
            ary[0] = "./slug";
            ary[1] = ct_string;
            ary[2] = NULL;
            break;
        }
    }

    int w;
    int status;
    int elapse;
    struct timespec before;
	clock_gettime(CLOCK_MONOTONIC, &before);
    if (PID != 0) {
        int program_ct = 4;
        while (program_ct > 0) {
            w = waitpid(-1, &status, WNOHANG);
            if (w != 0) {
                program_ct--;
                struct timespec after;
	            clock_gettime(CLOCK_MONOTONIC, &after);
	            elapse = after.tv_nsec - before.tv_nsec;
                printf("[Parent: %d]: the child %d finished in %d nanosec\n", PID, w, elapse);
            }
            usleep(33000);
        }
    }

    if (PID == 0) {
        //run slug.c with parameter
        execvp(cmd, ary);
    }

}