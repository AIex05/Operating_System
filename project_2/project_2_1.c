#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

// define semaphore
sem_t safe_sport; // one sport at a time on field
int sport = 0;
pthread_mutex_t mutexSport;

sem_t Enter_field;
sem_t Baseball_sem;
sem_t Baseball_limit;
sem_t Baseball_exit;
sem_t New_game;
sem_t Game_next_checkout;
sem_t Baseball_checkin;
pthread_mutex_t mutexBaseball;
pthread_cond_t condSport;
pthread_cond_t condField;
pthread_cond_t condBaseball;
pthread_cond_t condBaseball_exit;
pthread_cond_t condBaseball_exit_next;

int player_num_baseball = 0;

// define functions for players
void *baseball_func(void *arg)
{
    int *ID_c = (int *)arg;
    int ID = (int)*ID_c;
    while (1)
    {
        // Check game & Wait to clear field
        pthread_mutex_lock(&mutexSport);
        while (sport != 0)
        {
            pthread_cond_wait(&condSport, &mutexSport);
        }
        pthread_mutex_unlock(&mutexSport);

        // Limit & wait for players
        sem_wait(&Baseball_limit);
        sem_wait(&Baseball_checkin);
        pthread_mutex_lock(&mutexBaseball);
        player_num_baseball = player_num_baseball + 1;
        if (player_num_baseball == 18)
        {
            printf("[Baseball: %d]: Enough players w/ cleared field!\n", ID);
            pthread_cond_broadcast(&condBaseball);
            sem_post(&Enter_field);
        }
        else
        {
            while (player_num_baseball < 18)
            {
                printf("[Baseball: %d]: Entered the wait room..\n", ID);
                sem_post(&Baseball_checkin);
                pthread_cond_wait(&condBaseball, &mutexBaseball);
            }
        }
        pthread_mutex_unlock(&mutexBaseball);

        // Wait on exit signal
        sem_wait(&Baseball_exit);
        pthread_mutex_lock(&mutexBaseball);
        player_num_baseball = player_num_baseball - 1;
        if (player_num_baseball == 0)
        {
            printf("[Baseball: %d]: Last one exited..\n", ID);
            sem_post(&New_game);
            sem_post(&Baseball_checkin);
            sem_post(&Game_next_checkout);
        }
        else
        {
            printf("[Baseball: %d]: Exited the field..\n", ID);
            sem_post(&Game_next_checkout);
        }
        pthread_mutex_unlock(&mutexBaseball);
    }
}

void *football_func()
{
    while (1)
    {
    }
}

void *soccer_func()
{
    while (1)
    {
    }
}

void *game_func()
{
    while (1)
    {

        int player;
        if (sport == 0)
        {
            // baseball
            player = 18;
            for (int i = 0; i < player; i++) {
                sem_post(&Baseball_limit);
            }
        }
        else if (sport == 1)
        {
            // football
            // sem_post(&Football_sem);
        }
        else if (sport == 2)
        {
            // soccer
            // sem_post(&Soccer_sem);
        }

        sem_wait(&Enter_field);

        // Play game
        printf("[Field]: Now playing game...\n");
        sleep(3);

        if (sport == 0)
        {
            // baseball
            for (int c = 0; c < player; c++)
            {
                sem_post(&Baseball_exit);
                sem_wait(&Game_next_checkout);
            }
        }
        else if (sport == 1)
        {
            // football
            // sem_post(&Football_sem);
        }
        else if (sport == 2)
        {
            // soccer
            // sem_post(&Soccer_sem);
        }

        sem_wait(&New_game);
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
    pthread_cond_init(&condField, NULL);
    pthread_mutex_init(&mutexBaseball, NULL);
    pthread_cond_init(&condBaseball, NULL);
    pthread_cond_init(&condBaseball_exit, NULL);
    pthread_cond_init(&condBaseball_exit_next, NULL);
    sem_init(&Baseball_limit, 0, 0);
    sem_init(&Baseball_exit, 0, 0);
    sem_init(&Enter_field, 0, 0);
    sem_init(&New_game, 0, 0);
    sem_init(&Game_next_checkout, 0, 0);
    sem_init(&Baseball_checkin, 0, 1);

    pthread_t Game[1];
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
    pthread_create(&Game[0], NULL, &game_func, NULL);

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
}