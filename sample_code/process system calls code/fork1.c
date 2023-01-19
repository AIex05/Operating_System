
	/* Demonstrate the fork() system call (pg 221-222) */

#include <stdio.h>
#include <unistd.h>

main()
{
int i;

printf("Ready to fork...\n");
i=fork();
printf("Fork returned %d\n",i);
while (1);
}

