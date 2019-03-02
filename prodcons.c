/*
 *  prodcons module
 *  Producer Consumer module
 * 
 *  Implements routines for the producer consumer module based on 
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"


int count = 0;
int loops = LOOPS;
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;

// Define Locks and Condition variables here
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Producer consumer data structures

// Bounded buffer bigmatrix defined in prodcons.h
//Matrix ** bigmatrix;

// Bounded buffer put() get()
int put(Matrix * value)
{
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

Matrix * get() 
{
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
    int i;
    for (i = 0; i < loops; i++) 
    {
      pthread_mutex_lock(&mutex);
      while (count == MAX)
        pthread_cond_wait(&empty, &mutex);
    }
    put(i);
    pthread_cond_signal(&fill);
    pthread_cond_unlock(&mutex);
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  int i;
  for (i = 0; i < loops; i++) 
  {
    pthread_mutex_lock(&mutex);
    while (count == 0)
      pthread_cond_wait(&fill, &mutex);
    int tmp = get();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }
}

