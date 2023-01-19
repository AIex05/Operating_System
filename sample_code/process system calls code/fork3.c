
	/* fork() can be called iteratively (pgs 224-225) */

#include <stdio.h>
#include <unistd.h>

main()
{
int i;

i=getpid();
printf("Parent=%d\n",i);
fork();
fork();
i=getpid();
printf("Who am I? %d\n",i);
}

