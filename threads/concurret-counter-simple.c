#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "counter-simple.h"

#define NUM_ITER 1000000
#define MILLISEC 1000000

void * worker(void * args) {
    counter_simple_t * counter = args;
    for (int i = 0; i < NUM_ITER; i++) {
        update(counter);
    }

    pthread_exit(NULL);
}

double calc_time(struct timeval start, struct timeval end) {
    double start_sec = (double)start.tv_sec + (double)start.tv_usec / MILLISEC;
    double end_sec = (double)end.tv_sec + (double)end.tv_usec / MILLISEC;

    return end_sec - start_sec;
}

int main(int argc, char ** argv) {
    size_t num_cores = 1;
    if (argc < 2) {
        fprintf(stderr, "Usuage: ./counter [num_cores]\n");
        exit(EXIT_FAILURE);
    }

    num_cores = atoi(argv[1]);

    counter_simple_t counter;
    init(&counter);

    pthread_t * threads = malloc(sizeof(*threads) * num_cores);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < num_cores; i++) {
        pthread_create(&threads[i], NULL, worker, &counter);
    }

    for (size_t i  = 0; i < num_cores; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);
    
    free(threads);

    double time_elapsed = calc_time(start, end);
    printf("final value: %zu, run-time: %f\n", counter.counter, time_elapsed);
    exit(EXIT_SUCCESS);
}
