#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

// Create struct
struct baseball
{
    int ID;
    int playtime;
};

struct football
{
    int ID;
    int playtime;
};

struct soccer
{
    int ID;
    int playtime;
};

// define semaphore
sem_t Switch_game; // one sport at a time on field
sem_t Switch_game_finished;
int sport = 2;
pthread_mutex_t mutexSport;

sem_t Enter_field;
sem_t Baseball_limit;
sem_t Baseball_exit;
sem_t New_game;
sem_t Game_next_checkout;
sem_t Baseball_checkin;
pthread_mutex_t mutexBaseball;
pthread_cond_t condSport;
pthread_cond_t condBaseball;
clock_t Baseball_begin;

sem_t Football_limit;
sem_t Football_exit;
sem_t Football_checkin;
pthread_mutex_t mutexFootball;
pthread_cond_t condFootball;
clock_t Football_begin;

int player_num_baseball = 0;
int player_num_football = 0;
int player_num_soccer = 0;
int Soccer_first_time = 1;
int counter_soccer = 0;
int counter_soccer_out = 0;
int Soccer_time_up_bool = 0;
int Soccer_First_Time_time_up = 1;
int Baseball_play_T;
int Football_play_T;

sem_t Soccer_limit;
sem_t Soccer_checkin;
sem_t Soccer_two_player;
sem_t Soccer_checkout;
sem_t Soccer_two_player_out;
sem_t Soccer_firsttime;
sem_t Soccer_limit_2;
pthread_mutex_t lock_counter_soccer;
pthread_mutex_t lock_counter_soccer_out;
pthread_mutex_t Soccer_player_mutex;
pthread_mutex_t Soccer_First_Time_time_up_mu;
pthread_cond_t condSoccerTime;
pthread_mutex_t Soccer_time_mutex;
clock_t Soccer_begin;

// define functions for players
void *baseball_func(void *arg)
{
    struct baseball A = *(struct baseball *)arg;
    int ID = A.ID;
    if (ID == 35)
    {
        Baseball_begin = clock();
    }
    while (1)
    {
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
            if (((double)(clock() - Baseball_begin) / CLOCKS_PER_SEC) > 0.05)
            { // If time past change game!
                sem_post(&Switch_game);
                sem_wait(&Switch_game_finished);
            }
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

void *football_func(void *arg)
{
    struct football A = *(struct football *)arg;
    int ID = A.ID;
    if (ID == 43)
    {
        Football_begin = clock();
    }
    while (1)
    {
        // Limit & wait for players
        sem_wait(&Football_limit);
        sem_wait(&Football_checkin);
        pthread_mutex_lock(&mutexFootball);
        player_num_football = player_num_football + 1;
        if (player_num_football == 22)
        {
            printf("[Football: %d]: Enough players w/ cleared field!\n", ID);
            pthread_cond_broadcast(&condFootball);
            sem_post(&Enter_field);
        }
        else
        {
            while (player_num_football < 22)
            {
                printf("[Football: %d]: Entered the wait room..\n", ID);
                sem_post(&Football_checkin);
                pthread_cond_wait(&condFootball, &mutexFootball);
            }
        }
        pthread_mutex_unlock(&mutexFootball);

        // Wait on exit signal
        sem_wait(&Football_exit);
        pthread_mutex_lock(&mutexFootball);
        player_num_football = player_num_football - 1;
        if (player_num_football == 0)
        {
            printf("[Football: %d]: Last one exited..\n", ID);
            if (((double)(clock() - Football_begin) / CLOCKS_PER_SEC) > 0.05)
            { // If time past change game!
                sem_post(&Switch_game);
                sem_wait(&Switch_game_finished);
            }
            sem_post(&New_game);
            sem_post(&Football_checkin);
            sem_post(&Game_next_checkout);
        }
        else
        {
            printf("[Football: %d]: Exited the field..\n", ID);
            sem_post(&Game_next_checkout);
        }
        pthread_mutex_unlock(&mutexFootball);
    }
}

void *soccer_func(void *arg)
{
    struct soccer A = *(struct soccer *)arg;
    int ID = A.ID;
    int play_t = A.playtime;
    while (1)
    {
        sem_wait(&Soccer_limit);

        sem_wait(&Soccer_limit_2);

        // check in one by one
        sem_wait(&Soccer_checkin);
        pthread_mutex_lock(&lock_counter_soccer);
        if (counter_soccer == 0)
        {
            counter_soccer = counter_soccer + 1;
            pthread_mutex_unlock(&lock_counter_soccer);

            sem_post(&Soccer_checkin);
            sem_wait(&Soccer_two_player);

            while (((double)(clock() - Soccer_begin) / CLOCKS_PER_SEC) > 0.15)
            { // Time is up wait here
                pthread_mutex_lock(&Soccer_First_Time_time_up_mu);
                if (Soccer_First_Time_time_up == 1)
                {
                    Soccer_First_Time_time_up = 0;
                    Soccer_time_up_bool = 1;
                }
                pthread_mutex_unlock(&Soccer_First_Time_time_up_mu);
                pthread_cond_wait(&condSoccerTime, &Soccer_time_mutex);
            }

            pthread_mutex_lock(&Soccer_player_mutex);
            player_num_soccer = player_num_soccer + 2;
            pthread_mutex_unlock(&Soccer_player_mutex);

            sem_wait(&Soccer_firsttime);
            if (Soccer_first_time == 1)
            {
                Soccer_first_time = Soccer_first_time - 1;
                sem_post(&Enter_field);
            }
            sem_post(&Soccer_firsttime);

            sem_post(&Soccer_checkin);
        }
        else if (counter_soccer == 1)
        {
            counter_soccer = counter_soccer - 1;
            pthread_mutex_unlock(&lock_counter_soccer);
            sem_post(&Soccer_two_player);
        }

        printf("[Soccer: %d]: There are %d on field!\n", ID, player_num_soccer);

        // play for certain amount of time
        sleep(play_t);

        // checkout one by one
        sem_wait(&Soccer_checkout);
        pthread_mutex_lock(&lock_counter_soccer_out);
        if (counter_soccer_out == 0)
        {
            counter_soccer_out = counter_soccer_out + 1;
            pthread_mutex_unlock(&lock_counter_soccer_out);

            sem_post(&Soccer_checkout);

            sem_wait(&Soccer_two_player_out);
            pthread_mutex_lock(&Soccer_player_mutex);
            player_num_soccer = player_num_soccer - 2;
            pthread_mutex_lock(&Soccer_First_Time_time_up_mu);
            if (((double)(clock() - Soccer_begin) / CLOCKS_PER_SEC) > 0.15)
            { // time is up and field cleared
                if (player_num_soccer == 0)
                {
                    sem_post(&Switch_game);
                    sem_wait(&Switch_game_finished);
                    sem_post(&New_game);
                }
            }
            pthread_mutex_unlock(&Soccer_First_Time_time_up_mu);
            pthread_mutex_unlock(&Soccer_player_mutex);

            sem_post(&Soccer_checkout);
        }
        else if (counter_soccer_out == 1)
        {
            counter_soccer_out = counter_soccer_out - 1;
            pthread_mutex_unlock(&lock_counter_soccer_out);
            sem_post(&Soccer_two_player_out);
        }

        printf("[Soccer: %d]: Tired, I am quiting.. %d still on field\n", ID, player_num_soccer);

        sem_post(&Soccer_limit_2);
        sem_post(&Soccer_limit);
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
            for (int i = 0; i < player; i++)
            {
                sem_post(&Baseball_limit);
            }
        }
        else if (sport == 1)
        {
            // football
            player = 22;
            for (int i = 0; i < player; i++)
            {
                sem_post(&Football_limit);
            }
        }
        else if (sport == 2)
        {
            // soccer
            player = 22;
            Soccer_begin = clock();
            pthread_cond_broadcast(&condSoccerTime);
            for (int i = 0; i < player; i++)
            {
                sem_post(&Soccer_limit);
            }
        }

        sem_wait(&Enter_field);

        // Play game
        if (sport == 0)
        {
            // baseball
            printf("[Field]: Now playing Baseball...\n");
            sleep(Baseball_play_T);
        }
        else if (sport == 1)
        {
            // football
            printf("[Field]: Now playing Football...\n");
            sleep(Football_play_T);
        }
        else if (sport == 2)
        {
            // soccer
            printf("[Field]: Now playing Soccer...\n");
        }

        // Exit field
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
            for (int c = 0; c < player; c++)
            {
                sem_post(&Football_exit);
                sem_wait(&Game_next_checkout);
            }
        }
        else if (sport == 2)
        {
            // soccer
            sleep(0.3);
        }

        sem_wait(&New_game);

        sem_wait(&Soccer_firsttime);
        if (Soccer_first_time == 0)
        {
            Soccer_first_time = Soccer_first_time + 1;
        }
        sem_post(&Soccer_firsttime);
    }
}

void *Game_change_func()
{
    while (1)
    {
        sem_wait(&Switch_game);
        pthread_mutex_lock(&mutexSport);
        if (sport == 0)
        {
            // baseball
            Football_begin = clock();
            sport = 1;
        }
        else if (sport == 1)
        {
            // football
            sport = 2;
        }
        else if (sport == 2)
        {
            // soccer
            Baseball_begin = clock();
            sport = 0;
        }
        pthread_mutex_unlock(&mutexSport);
        sem_post(&Switch_game_finished);
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

    sem_init(&Switch_game, 0, 0);
    pthread_mutex_init(&mutexSport, NULL);
    pthread_cond_init(&condSport, NULL);
    pthread_mutex_init(&mutexBaseball, NULL);
    pthread_cond_init(&condBaseball, NULL);
    sem_init(&Baseball_limit, 0, 0);
    sem_init(&Baseball_exit, 0, 0);
    sem_init(&Enter_field, 0, 0);
    sem_init(&New_game, 0, 0);
    sem_init(&Game_next_checkout, 0, 0);
    sem_init(&Baseball_checkin, 0, 1);
    sem_init(&Switch_game_finished, 0, 0);

    pthread_mutex_init(&mutexFootball, NULL);
    pthread_cond_init(&condFootball, NULL);
    sem_init(&Football_limit, 0, 0);
    sem_init(&Football_exit, 0, 0);
    sem_init(&Football_checkin, 0, 1);

    sem_init(&Soccer_limit, 0, 0);
    sem_init(&Soccer_limit_2, 0, 22);
    pthread_mutex_init(&lock_counter_soccer, NULL);
    pthread_mutex_init(&Soccer_player_mutex, NULL);
    pthread_mutex_init(&lock_counter_soccer, NULL);
    sem_init(&Soccer_checkin, 0, 1);
    sem_init(&Soccer_checkout, 0, 1);
    sem_init(&Soccer_two_player, 0, 0);
    sem_init(&Soccer_two_player_out, 0, 0);
    sem_init(&Soccer_firsttime, 0, 1);
    pthread_cond_init(&condSoccerTime, NULL);
    pthread_mutex_init(&Soccer_time_mutex, NULL);

    pthread_t Game[1];
    pthread_t Swithc_Game[1];
    pthread_t baseball[36];
    pthread_t football[44];
    pthread_t soccer[44];

    struct baseball Baseball[36];
    struct football Football[44];
    struct soccer Soccer[44];

    // Random play number for Baseball and football
    Baseball_play_T = rand() % 4 + 1;
    Football_play_T = rand() % 4 + 1;

    // create threads
    for (int i = 0; i < 44; i++)
    {
        if (i < 36)
        {
            Baseball[i].ID = i;
            Baseball[i].playtime = 0;
            pthread_create(&baseball[i], NULL, &baseball_func, &Baseball[i]);
        }
        Football[i].ID = i;
        Football[i].playtime = 0;
        pthread_create(&football[i], NULL, &football_func, &Football[i]);
        Soccer[i].ID = i;
        Soccer[i].playtime = rand() % 3 + 1;
        pthread_create(&soccer[i], NULL, &soccer_func, &Soccer[i]);
    }
    pthread_create(&Game[0], NULL, &game_func, NULL);
    pthread_create(&Swithc_Game[0], NULL, &Game_change_func, NULL);

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
}