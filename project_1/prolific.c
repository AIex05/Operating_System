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
	for (int k=0;k<13;k++) {
		Random_number[k] = rand();
	}
	FILE *fp = fopen("seed.txt", "r");
	while(fscanf(fp,"%s",seed)!=EOF){
		printf("The seed is %s\n",seed);
	}
	fclose(fp);
	seed_int = atoi(seed);
	srand(seed_int);
	int upper = 13, lower = 8, Rand_Num;
	Random(upper, lower, &Rand_Num);
	printf("Number of processes are %d\n", Rand_Num);

	int PID = 1;
	for (counter = 0; counter < Rand_Num; counter++) {
		if (PID != 0) {
			int id = getpid();
			//printf("The parent process ID is %d\n", id);
			PID = fork();
			waitpid(PID, NULL, 0);
		} else {
			int id = getpid();
			int my_num = Random_number[counter];
			int Exit_code = ((my_num%50)+1);
			int Wait_time = ((my_num%3)+1);
			printf("This is child %d\n    My random number: %d\n    My wait time: %d\n    My exit code: %d\n", id, my_num, Wait_time, Exit_code);
			sleep(Wait_time);
			exit(Exit_code);
		}
	}
}
