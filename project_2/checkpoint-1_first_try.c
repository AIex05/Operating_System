#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

// define semaphore
sem_t safe_sport;       // one sport at a time on field
int sport = 0;
int Baseball_wait = 0;
int Baseball_mid = 0;
int Baseball_fin = 0;
int Football_wait = 0;
int Soccer_wait = 0;

pthread_cond_t condSport;
pthread_mutex_t mutexSport;

pthread_cond_t condBaseball;
pthread_mutex_t mutexBaseball;
pthread_cond_t condBaseball_mid;
pthread_mutex_t mutexBaseball_mid;
pthread_cond_t condBaseball_fin;
pthread_mutex_t mutexBaseball_fin;
pthread_cond_t condFootball;
pthread_mutex_t mutexFootball;
pthread_cond_t condSoccer;
pthread_mutex_t mutexSoccer;

sem_t Baseball_sem;

int player_num_baseball;

// define functions for players
void *baseball_func(void *arg)
{
    int *ID_c = (int *)arg;
    int ID = (int) *ID_c;
    while (1)
    {
        //Check game
        pthread_mutex_lock(&mutexSport);
        while (sport != 0) {
            printf("[T=%d]: Not baseball yet...\n", ID);
            pthread_cond_wait(&condSport, &mutexSport);
        }
        pthread_mutex_unlock(&mutexSport);

        //Check time
        // if time is up, change a game 
        // pthread_mutex_lock(&mutexSport);
        // if (t > 10s){
        //     Sport = 1;
        //      wait on specific signal
        //      pthread_cond_broadcast(%condSport);
        // }
        // pthread_mutex_unlock(&mutexSport);

        //18 persons yet?
        sem_wait(&Baseball_sem);
        pthread_mutex_lock(&mutexBaseball);
        if (Baseball_wait == 17) {
            printf("[T=%d]: BB player num reached\n",ID);
            Baseball_wait = Baseball_wait + 1;
            pthread_cond_broadcast(&condBaseball);
        } else {
            Baseball_wait = Baseball_wait + 1;
        }
        printf("[T=%d]: %d in waiting area 1 ..\n",ID, Baseball_wait);
        while (Baseball_wait < 18) {
            pthread_cond_wait(&condBaseball, &mutexBaseball);
        }
        pthread_mutex_unlock(&mutexBaseball);

        printf("[T=%d]: Baseball player playing\n",ID);
        //Play time
        sleep(5);

        pthread_mutex_lock(&mutexBaseball_mid);
        if (Baseball_mid == 17) {
            Baseball_mid = Baseball_mid + 1;
            pthread_cond_broadcast(&condBaseball_mid);
        }else {
            Baseball_mid = Baseball_mid + 1;
        }
        while (Baseball_mid < 18)
        {
            pthread_cond_wait(&condBaseball_mid, &mutexBaseball_mid);
        }
        pthread_mutex_unlock(&mutexBaseball_mid);

        pthread_mutex_lock(&mutexBaseball);
        Baseball_wait = Baseball_wait - 1;
        printf("[T=%d]: %d I'm done..\n",ID, Baseball_wait);
        pthread_mutex_unlock(&mutexBaseball);

        pthread_mutex_lock(&mutexBaseball_fin);
        if ((Baseball_fin % 18) == 17) {
            Baseball_fin = Baseball_fin + 1;
            pthread_cond_broadcast(&condBaseball_fin);
        }else {
            Baseball_fin = Baseball_fin + 1;
        }
        while (Baseball_fin < 18)
        {
            pthread_cond_wait(&condBaseball_fin, &mutexBaseball_fin);
        }
        pthread_mutex_unlock(&mutexBaseball_fin);

        pthread_mutex_lock(&mutexBaseball_mid);
        Baseball_mid = Baseball_mid - 1;
        pthread_mutex_unlock(&mutexBaseball_mid);

        sem_post(&Baseball_sem);

        //After game rest
        sleep(3);
    }
}

void *football_func()
{
    while (1)
    {
        pthread_mutex_lock(&mutexSport);
        while (sport != 1) {
            //printf("Not football yet...\n");
            pthread_cond_wait(&condSport, &mutexSport);
        }
        pthread_mutex_unlock(&mutexSport);

        //Time constraint

        //Check for enough player
        pthread_mutex_lock(&mutexFootball);
        Football_wait = Football_wait + 1;
        while (Football_wait != 18) {
            printf("Not enough football player...");
            pthread_cond_wait(&condFootball, &mutexFootball);
        }
        pthread_cond_broadcast(&condFootball);
        pthread_mutex_unlock(&mutexFootball);

        pthread_mutex_lock(&mutexFootball);
        Football_wait = Football_wait - 1;
        printf("Football player playing\n");
        pthread_mutex_unlock(&mutexFootball);

        sleep(3);
    }
}

void *soccer_func()
{
    while (1)
    {
        pthread_mutex_lock(&mutexSport);
        while (sport != 2) {
            //printf("Not soccer turn yet...\n");
            pthread_cond_wait(&condSport, &mutexSport);
        }
        pthread_mutex_unlock(&mutexSport);

        printf("Soccer player playing\n");
    }
}

int main()
{
    int seed_int, counter;
    char seed[10000];
    FILE *fp = fopen("seed.txt", "r");
    while (fscanf(fp, "%s", seed) != EOF)
    {
        printf("The seed is %s\n", seed);
    }
    fclose(fp);
    seed_int = atoi(seed);
    srand(seed_int);

    sem_init(&safe_sport, 0, 1);
    pthread_mutex_init(&mutexSport, NULL);
    pthread_cond_init(&condSport, NULL);
    pthread_mutex_init(&mutexBaseball, NULL);
    pthread_cond_init(&condBaseball, NULL);
    pthread_mutex_init(&mutexBaseball_fin, NULL);
    pthread_cond_init(&condBaseball_fin, NULL);
    pthread_mutex_init(&mutexBaseball_mid, NULL);
    pthread_cond_init(&condBaseball_mid, NULL);
    sem_init(&Baseball_sem, 0, 18);

    pthread_t baseball[36];
    pthread_t football[44];
    pthread_t soccer[44];

    // create threads
    for (int i = 0; i < 44; i++)
    {
        if (i < 36)
        {
            pthread_create(&baseball[i], NULL, &baseball_func, &i);
        }
        pthread_create(&football[i], NULL, &football_func, NULL);
        pthread_create(&soccer[i], NULL, &soccer_func, NULL);
    }

    // Join threads
    for (int i = 0; i < 44; i++)
    {
        if (i < 36)
        {
            pthread_join(baseball[i], NULL);
        }
        pthread_join(football[i], NULL);
        pthread_join(soccer[i], NULL);
    }

    sem_destroy(&safe_sport);
    pthread_mutex_destroy(&mutexSport);
    pthread_cond_destroy(&condSport);
}