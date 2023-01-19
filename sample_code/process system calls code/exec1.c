
	/* Demonstrate the exec() system call (pgs 225-226) */

#include <stdio.h>
#include <unistd.h>

main()
{
char program[80],*args[3];
int i;

printf("Ready to exec()...\n");
strcpy(program,"date");
args[0]="date";
args[1]="-u";
args[2]=NULL;
i=execvp(program,args);
printf("i=%d ... did it work?\n",i);
}

