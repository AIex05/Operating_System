
	/* Child process can run different code than parent process
	** (pgs 223-224) */

#include <stdio.h>
#include <unistd.h>

main()
{
int i,j;
j=0;

printf("Ready to fork...\n");
i=fork();
if (i == 0)
  {
  printf("The child executes this code.\n");
  for (i=0; i<5; i++)
    j=j+i;
  printf("Child j=%d\n",j);
  }
else
  {
  printf("The parent executes this code.\n");
  for (i=0; i<3; i++)
    j=j+i;
  printf("Parent j=%d\n",j);
  }
}

