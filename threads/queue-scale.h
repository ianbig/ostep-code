#ifndef QUEUE_SCALE_H
#define QUEUE_SCALE_H

#include <pthread.h>
#include <stdlib.h>

typedef struct _node_t node_t;

typedef struct _node_t {
  int val;
  node_t * next;
} node_t;

typedef struct _queue_t {
  node_t * head;
  node_t * tail;
  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
} queue_t;


void qu_init(queue_t * qu) {
  node_t * headDummy = (node_t*)malloc(sizeof(*headDummy));

  headDummy->next = NULL;
  headDummy->val = -1;

  qu->head = headDummy;
  qu->tail = headDummy;

  pthread_mutex_init(&qu->head_lock, NULL);
  pthread_mutex_init(&qu->tail_lock, NULL);
}

int enqueue(queue_t * qu, int val) {
  node_t * tmp = (node_t*)malloc(sizeof(*tmp));
  if (tmp == NULL) {
    return -1;
  }
  tmp->val = val;
  tmp->next = NULL;

  pthread_mutex_lock(&qu->tail_lock);
  qu->tail->next = tmp;
  qu->tail = tmp;
  pthread_mutex_unlock(&qu->tail_lock);

  return 0;
}


int dequeue(queue_t * qu) {
  pthread_mutex_lock(&qu->head_lock);
  node_t * to_pop = qu->head;
  node_t * new_head = to_pop->next;
  if (new_head == NULL) {
    pthread_mutex_unlock(&qu->head_lock);
    return -1;
  }
  int ret = new_head->val;
  to_pop->next = NULL;
  qu->head = new_head;
  pthread_mutex_unlock(&qu->head_lock);

  free(to_pop);
  return ret;
}

void qu_destroy(queue_t * qu) {
  
}

#endif