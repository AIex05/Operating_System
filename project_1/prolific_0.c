#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

void Random (int upper, int lower, int *pointer) {
	*pointer = (rand() % (upper - lower + 1)) + lower;
}

int main () {
	int lower = 8, upper = 13, i;
	char seed[100000];
	FILE *fp = fopen("seed.txt", "r");
	srand(time(0));
	Random(upper, lower, &i);
	int process_id = fork();
	if (process_id != 0) {
		for (int count = 0; count < i; count++) {
			if (process_id != 0) {
				process_id = fork();
			} else {
				//Child intro
				int PID = getpid();
                		printf("This is child %d!\n", PID);
				wait(NULL);
				exit(0);
			}
		}
	} else {
		//Child intro
        	printf("This is child!\n");
		wait(NULL);
		exit(0);
	}
	wait(NULL);
	exit(0);
}
