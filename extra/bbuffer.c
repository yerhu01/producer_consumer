/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bbuffer.h"

#include <semaphore.h>

/*
 * Declarations for bounded-buffer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

/* Arrays declared as global (or static locally) are initalized to 0 */
int bounded_buffer[BOUNDED_BUFFER_SIZE];
sem_t empty; /* number of empty slots */
sem_t occupied; /* number of occupied slots */
sem_t mutex; /* for multiple clients (try example without this)  */

/* Stuff for barrier */
pthread_barrier_t barrier;

void initialize_bounded_buffer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    int status; 

    /* empty initialized to BOUNDED_BUFFER_SIZE */
    status = sem_init(&empty, 0, BOUNDED_BUFFER_SIZE);
    if(status != 0){
        fprintf(stderr, "Error creating empty\n");
    	exit(1);
    }

    /* occupied initialized to 0 */
    status = sem_init(&occupied, 0, 0);
    if(status != 0){
        fprintf(stderr, "Error creating occupied\n");
        exit(1);
    }

    /* mutex  initialized to 1 */
    status = sem_init(&mutex, 0, 1);
    if(status != 0){
        fprintf(stderr, "Error creating occupied\n");
        exit(1);
    }

    /* Stuff for Barrier */
    status = pthread_barrier_init(&barrier,NULL,3); 
    if(status != 0){
        fprintf(stderr, "Error creating barrier\n");
        exit(1);
    }
}


void add_to_buffer(int value) {
    /* TO DO */
    pthread_barrier_wait(&barrier); /* requires 3 clients  requesting to add, before adding*/
    sem_wait(&empty); /* wait until at least one empty slot */
    /* empty value reduced by one */
    sem_wait(&mutex);

    /* place value in next available "in" position in buffer */
    int in;
    for(in = 0; in < BOUNDED_BUFFER_SIZE; in++){
        if(bounded_buffer[in] == 0){
	    break;
        }
    }
    bounded_buffer[in] = value;
  
    printf("after add: ");
    int i;
    for(i = 0; i < BOUNDED_BUFFER_SIZE; i++){
        printf("%d ", bounded_buffer[i]);
    }
    printf("\n");

    sem_post(&mutex);  
    sem_post(&occupied); /* indicate there is one more occupied slot */
}


int remove_from_buffer() {
    /* TO DO */    
    sem_wait(&occupied); /* wait until at least one occupied slot */
    /* occupied value reduced by one */

    sem_wait(&mutex);

    /* remove value at next available "out" position */
    int out;
    for(out = 0; out < BOUNDED_BUFFER_SIZE; out++){
        if(bounded_buffer[out] != 0){
            break;
        }
    }
    int value = bounded_buffer[out];
    bounded_buffer[out] = 0;
    
    printf("after removal: ");
    int i;
    for(i = 0; i < BOUNDED_BUFFER_SIZE; i++){
        printf("%d ", bounded_buffer[i]);
    }
    printf("\n");
    
    sem_post(&mutex);
    sem_post(&empty); /* indicates there is one more empty slot */

    /* return value to client */
    return value;
}
