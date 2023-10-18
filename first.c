#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the maximum number of threads
#define MAX_THREADS 100

void *philosopherThread(void *threadIndex) {
    int index = *((int *)threadIndex);
    printf("This is philosopher %d\n", index);
    return NULL;
}

void createPhilosophers(int nthreads) {
    pthread_t threads[MAX_THREADS];
    int threadIndex[MAX_THREADS];

    if (nthreads > MAX_THREADS) {
        printf("Error: Too many threads requested.\n");
        return;
    }

    for (int i = 0; i < nthreads; i++) {
        threadIndex[i] = i;
        if (pthread_create(&threads[i], NULL, philosopherThread, &threadIndex[i])) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d threads have been completed/joined successfully!\n", nthreads);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int nthreads = atoi(argv[1]);
    if (nthreads <= 0) {
        printf("Please provide a valid number of threads.\n");
        return 1;
    }

    printf("Assignment 7: # of threads = %d\n", nthreads);

    createPhilosophers(nthreads);

    return 0;
}
