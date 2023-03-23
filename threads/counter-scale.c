#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "counter-scale.h"

#define NUM_ITER 1000000
#define MILLISEC 1000000

size_t num_cores = 1;
int *isTake = NULL;

void * worker(void * args) {
    counter_scale_t * counter = args;

    size_t index = 0;
    pthread_mutex_t lck = PTHREAD_MUTEX_INITIALIZER;
    for (size_t i = 0; i < num_cores; i++) {
      pthread_mutex_lock(&lck);
      if (isTake[i] == 0) {
        index = i;
        isTake[i] = 1;
        pthread_mutex_unlock(&lck);
        break;
      }
      pthread_mutex_unlock(&lck);
    }

    for (int i = 0; i < NUM_ITER; i++) {
        update(counter, index, num_cores);
    }

    pthread_exit(NULL);
}

double calc_time(struct timeval start, struct timeval end) {
    double start_sec = (double)start.tv_sec + (double)start.tv_usec / MILLISEC;
    double end_sec = (double)end.tv_sec + (double)end.tv_usec / MILLISEC;

    return end_sec - start_sec;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usuage: ./counter [num_cores]\n");
        exit(EXIT_FAILURE);
    }

    num_cores = atoi(argv[1]);
    counter_scale_t counter;
    init(&counter, num_cores);

    isTake = malloc(sizeof(*isTake) * num_cores);
    for (size_t i = 0; i < num_cores; i++) {
      isTake[i] = 0;
    }

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
    printf("final value: %zu, run-time: %f\n", counter.gcounter, time_elapsed);
    destroy(&counter);
    free(isTake);
    exit(EXIT_SUCCESS);
}
