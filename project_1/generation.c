#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

void Random (int upper, int lower, int *pointer) {
	*pointer = (rand() % (upper - lower + 1)) + lower;
}

int main () {
	char seed[10000];
	int Random_number[13];
	int seed_int, counter;
	FILE *fp = fopen("seed.txt", "r");
	while(fscanf(fp,"%s",seed)!=EOF){
		printf("The seed is %s\n",seed);
	}
	fclose(fp);
	seed_int = atoi(seed);
	srand(seed_int);
	int upper = 12, lower = 5, Rand_Num, Rand_Num_cp;
	Random(upper, lower, &Rand_Num);
	printf("Number of processes are %d\n", Rand_Num);
	Rand_Num_cp = Rand_Num;
	
	int PID = 0;
	while (Rand_Num_cp > 0) {
		if (PID == 0) {
			Rand_Num_cp = Rand_Num_cp - 1;
			PID = fork();
		}
		if (PID == 0) {
			int my_pid = getpid();
			printf("[Child, PID: %d]: Descendant count %d, will have %d descendant\n", my_pid, (Rand_Num_cp+1), Rand_Num_cp);
			//sleep(0.5);
		}
		if (PID != 0) {
                        int status;
                        int my_pid = getpid();
                        printf("[Parent, PID: %d]: Waiting for %d\n", my_pid, PID);
                        waitpid(PID, &status, 0);
			int exit_code = WEXITSTATUS(status);
			printf("[Parent, PID: %d]: Child exit code %d, I exit\n", my_pid, exit_code);
			exit(Rand_Num_cp+1);
                }
	}
}
