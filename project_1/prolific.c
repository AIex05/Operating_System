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
	for (int k=0;k<13;k++) {
                Random_number[k] = rand();
        }
	int upper = 13, lower = 8, Rand_Num;
	Random(upper, lower, &Rand_Num);
	printf("Number of processes are %d\n", Rand_Num);

	int PID = 1;
	for (counter = 0; counter < Rand_Num; counter++) {
		if (PID != 0) {
			int status;
			int id = getpid();
			PID = fork();
			if (PID != 0) {
				printf("[Parent]: Waiting for %d to finish!\n", PID);
			}
			waitpid(PID, &status, 0);
			if (PID !=0) {
				int exit_cd = WEXITSTATUS(status);
				printf("[Parent]: Process %d finished with code %d\n", PID, exit_cd);
			}
		} else {
			int id = getpid();
			int my_num = Random_number[counter];
			int Exit_code = ((my_num%50)+1);
			int Wait_time = ((my_num%3)+1);
			printf("	[Child, PID = %d]: I am child, wait %d seconds, exit code %d.\n", id, Wait_time, Exit_code);
			sleep(Wait_time);
			printf("	[Child, PID = %d]: Now exiting ..\n", id);
			exit(Exit_code);
		}
	}
	if (counter == Rand_Num && PID == 0) {
		int id = getpid();
                int my_num = Random_number[counter];
                int Exit_code = ((my_num%50)+1);
                int Wait_time = ((my_num%3)+1);
                printf("        [Child, PID = %d]: I am child, wait %d seconds, exit code %d.\n", id, Wait_time, Exit_code);
                sleep(Wait_time);
                printf("        [Child, PID = %d]: Now exiting ..\n", id);
                exit(Exit_code);
	}
}
