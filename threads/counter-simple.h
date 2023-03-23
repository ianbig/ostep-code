#ifndef __COUNTER_SIMPLE_H
#define __COUNTER_SIMPLE_H

#include <pthread.h>
#include <stdio.h>

typedef struct _counter_simple_t {
    pthread_mutex_t lck;
    size_t counter;
} counter_simple_t;


void init(counter_simple_t * counter) {
    pthread_mutex_init(&counter->lck, NULL);
    counter->counter = 0;
}

void update(counter_simple_t * counter) {
    pthread_mutex_lock(&counter->lck);
    counter->counter++;
    pthread_mutex_unlock(&counter->lck);
}

#endif
