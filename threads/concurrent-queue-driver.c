#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "queue-scale.h"

#define NUM_ITER 100
queue_t qu;

void * push_worker(void * args) {
  for (int i = 0; i < NUM_ITER; i++) {
    enqueue(&qu, i);
  }
  pthread_exit(NULL);
}

void* pop_worker(void * args) {
  for (int i = 0; i < NUM_ITER; i++) {
    int a = dequeue(&qu);
    printf("we get %d\n", a);
  }

  pthread_exit(NULL);
}


int main() {
  // TODO: come up with a test plan
  qu_init(&qu);

  int num_cores = 2;
  pthread_t threads[2];
  pthread_create(&threads[0], NULL, push_worker, NULL);
  pthread_create(&threads[1], NULL, pop_worker, NULL);

  for (int i = 0; i < num_cores; i++) {
    pthread_join(threads[i], NULL);
  }
}