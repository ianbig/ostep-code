#ifndef __COUNTER_SCALE_H
#define __COUNTER_SCALE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREASHOLD 1000000

typedef struct _counter_scale_t
{
  int * lcounter;
  pthread_mutex_t * llck;
  size_t gcounter;
  pthread_mutex_t glock;
} counter_scale_t;

void init(counter_scale_t * counter, size_t num_cores) {
  counter->lcounter = (int *)malloc(sizeof(*counter->lcounter) * num_cores);
  counter->llck = (pthread_mutex_t *)malloc(sizeof(*counter->llck) * num_cores);
  counter->gcounter = 0;
  pthread_mutex_init(&counter->glock, NULL);
  for (size_t i = 0; i < num_cores; i++) {
    counter->lcounter[i] = 0;
    pthread_mutex_init(&counter->llck[i], NULL);
  }
}

void update(counter_scale_t * counter, pthread_t thread_id, size_t num_cores) {
  size_t lcounter_index = (size_t)thread_id % num_cores;
  pthread_mutex_lock(&counter->llck[lcounter_index]);
  if (counter->lcounter[lcounter_index] == THREASHOLD - 1) {
    pthread_mutex_lock(&counter->glock);
    counter->lcounter[lcounter_index]++;
    counter->gcounter += counter->lcounter[lcounter_index];
    counter->lcounter[lcounter_index] = 0;
    pthread_mutex_unlock(&counter->glock);
  } else {
    counter->lcounter[lcounter_index]++;
  }
  pthread_mutex_unlock(&counter->llck[lcounter_index]);
}

void destroy(counter_scale_t * counter) {
  free(counter->llck);
  free(counter->lcounter);
}

#endif