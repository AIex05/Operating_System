#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void Random(int upper, int lower, int *pointer)
{
	*pointer = (rand() % (upper - lower + 1)) + lower;
}

void main(int argc, char *argv[])
{

	char slugseed[10000];

	// int secArg;
	int slugseed_int;
	int PID = getpid();
	printf("%s\n", argv[1]);

	// secArg = *argv[1];
	// printf("%d\n", secArg);

	if (argc != 2)
	{
		printf("[Slug: %d]: usage syntax: %s (1/2/3/4)",PID , argv[0]);
	}
	else if (!strcmp(argv[1], "1"))
	{

		FILE *fileP = fopen("seed_slug_1.txt", "r");
		while (fscanf(fileP, "%s", slugseed) != EOF)
		{
			printf("[Slug: %d]: Read seed value: %s\n",PID, slugseed);
		}
		fclose(fileP);
	}
	else if (!strcmp(argv[1], "2"))
	{

		FILE *fileP = fopen("seed_slug_2.txt", "r");
		while (fscanf(fileP, "%s", slugseed) != EOF)
		{
			printf("[Slug: %d]: Read seed value: %s\n",PID, slugseed);
		}
		fclose(fileP);
	}
	else if (!strcmp(argv[1], "3"))
	{

		FILE *fileP = fopen("seed_slug_3.txt", "r");
		while (fscanf(fileP, "%s", slugseed) != EOF)
		{
			printf("[Slug: %d]: Read seed value: %s\n",PID, slugseed);
		}
		fclose(fileP);
	}
	else if (!strcmp(argv[1], "4"))
	{

		FILE *fileP = fopen("seed_slug_4.txt", "r");
		while (fscanf(fileP, "%s", slugseed) != EOF)
		{
			printf("[Slug: %d]: Read seed value: %s\n", PID, slugseed);
		}
		fclose(fileP);
	}
	else
		printf("something went wrong");

	slugseed_int = atoi(slugseed);
	printf("[Slug: %d]: Read seed value (converted to integer): %d\n",PID, slugseed_int);
	srand(slugseed_int);

	int delayUpper = 6, delayLower = 1, delay_rNum;
	int coinflipUpper = 2, coinflipLower = 1, coinflip_rNum;

	Random(delayUpper, delayLower, &delay_rNum);
	Random(coinflipUpper, coinflipLower, &coinflip_rNum);
	int wait_time = delay_rNum;
	int id = getpid();

	printf("[Slug: %d]: Delay time is %d seconds. Coin flip: %d\n",PID, delay_rNum, coinflip_rNum);
	sleep(wait_time);

	int status;
	FILE *fileP;
	int PATH_MAX = 130;
	char path[PATH_MAX];

	if (coinflip_rNum == 1)
	{

		printf("[Slug: %d]: Break time is over! I am running the 'last -i -x' command\n",PID);
		fileP = popen("last -i -x", "r");
		if (fileP == NULL)
			printf("NULL error");

		while (fgets(path, PATH_MAX, fileP) != NULL)
			printf("%s\n", path);

		status = pclose(fileP);
		if (status == -1)
		{
			printf("[Slug: %d]: pclose() error",PID);
		}
		else
		{
			printf("[Slug: %d]: success",PID);
		}
	}
	else
	{

		printf("[Slug: %d]: Break time is over! I am running the 'id --group' command\n",PID);
		fileP = popen("id --group", "r");
		if (fileP == NULL)
			printf("NULL error");

		while (fgets(path, PATH_MAX, fileP) != NULL)
			printf("[Slug: %d]: %s\n",PID, path);

		status = pclose(fileP);
		if (status == -1)
		{
			printf("pclose() error");
		}
		else
		{
			printf("success");
		}
	}

	/*srand(time(0));
	Random(*/
}