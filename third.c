#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t chopstick[NUM_PHILOSOPHERS];
pthread_mutex_t globalMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t nextCond = PTHREAD_COND_INITIALIZER;

int nextIndex = 0;

void thinking(int philosopher_num) {
    printf("Philosopher #%d is thinking.\n", philosopher_num);
    usleep((rand() % 500 + 1) * 1000);
}

void eating(int philosopher_num) {
    printf("Philosopher #%d is eating.\n", philosopher_num);
    usleep((rand() % 500 + 1) * 1000);
}

void pickUpChopsticks(int philosopher_num) {
    int left = philosopher_num;
    int right = (philosopher_num + 1) % NUM_PHILOSOPHERS;

    pthread_mutex_lock(&globalMutex);
    
    while (nextIndex != philosopher_num) {
        pthread_cond_wait(&nextCond, &globalMutex);
    }

    pthread_mutex_unlock(&globalMutex);

    pthread_mutex_lock(&chopstick[left]);
    pthread_mutex_lock(&chopstick[right]);

    printf("Philosopher #%d picked up chopsticks %d and %d.\n", philosopher_num, left, right);
}

void putDownChopsticks(int philosopher_num) {
    int left = philosopher_num;
    int right = (philosopher_num + 1) % NUM_PHILOSOPHERS;

    pthread_mutex_lock(&globalMutex);

    nextIndex = (nextIndex + 1) % NUM_PHILOSOPHERS;
    pthread_cond_broadcast(&nextCond);

    pthread_mutex_unlock(&globalMutex);

    pthread_mutex_unlock(&chopstick[left]);
    pthread_mutex_unlock(&chopstick[right]);

    printf("Philosopher #%d put down chopsticks %d and %d.\n", philosopher_num, left, right);
}

void *philosopherThread(void *arg) {
    int philosopher_num = *((int *)arg);

    while (1) {
        thinking(philosopher_num);
        pickUpChopsticks(philosopher_num);
        eating(philosopher_num);
        putDownChopsticks(philosopher_num);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int i;
    int philosopher_num[NUM_PHILOSOPHERS];

    srand(time(NULL));

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_num[i] = i;
        if (pthread_create(&philosophers[i], NULL, philosopherThread, &philosopher_num[i]) != 0) {
            fprintf(stderr, "Error creating philosopher thread #%d\n", i);
            exit(1);
        }
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}
