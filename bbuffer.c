/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bbuffer.h"

/*
 * Declarations for bounded-buffer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

/* Arrays declared as global (or static locally) are initalized to 0 */
int bounded_buffer[BOUNDED_BUFFER_SIZE];
int count = 0;
pthread_mutex_t buffer_mutex;
pthread_cond_t cond_add; 
pthread_cond_t cond_rem;

void initialize_bounded_buffer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    int status; 

    status = pthread_mutex_init(&buffer_mutex, NULL);
    if(status != 0){
        fprintf(stderr, "Error creating buffer_mutex\n");
    	exit(1);
    }

    status = pthread_cond_init(&cond_add, NULL);
    if(status != 0){
        fprintf(stderr, "Error creating cond_add\n");
        exit(1);
    }

    status = pthread_cond_init(&cond_rem, NULL);
    if(status != 0){
    	fprintf(stderr, "Error creating cond_rem\n");
        exit(1);
    }
}


void add_to_buffer(int value) {
    /* TO DO */

    pthread_mutex_lock(&buffer_mutex); /* protect buffer */
    while (count == BOUNDED_BUFFER_SIZE){   /* guard: no available space */
    	pthread_cond_wait(&cond_add, &buffer_mutex);  /* wait until at least one empty slot in buffer */
    }

  
    /* place value in next available "in" position in buffer */
    int in;
    for(in = 0; in < BOUNDED_BUFFER_SIZE; in++){
        if(bounded_buffer[in] == 0){
            break;
        }
    }
    bounded_buffer[in] = value;
    count = count + 1;
    
    /*
    printf("after add: ");
    int i;
    for(i = 0; i < BOUNDED_BUFFER_SIZE; i++){
        printf("%d ", bounded_buffer[i]);
    }
    printf("\n");
    */

    pthread_cond_signal(&cond_rem); /* wake up removing thread */
    pthread_mutex_unlock(&buffer_mutex);
}

int remove_from_buffer() {
    /* TO DO */
   
    pthread_mutex_lock(&buffer_mutex);
    while(count == 0){ /* guard: nothing to remove */
    	pthread_cond_wait(&cond_rem, &buffer_mutex); /* wait until at least one value in buffer */
    }

    /* remove value at next available "out" position */
    int out;
    for(out = 0; out < BOUNDED_BUFFER_SIZE; out++){
        if(bounded_buffer[out] != 0){
            break;
        }
    }
    int value = bounded_buffer[out];
    bounded_buffer[out] = 0;
    count = count - 1;
    
    /*
    printf("after removal: ");
    int i;
    for(i = 0; i < BOUNDED_BUFFER_SIZE; i++){
        printf("%d ", bounded_buffer[i]);
    }
    printf("\n");
    */

    pthread_cond_signal(&cond_add); /* wake up adding thread */
    pthread_mutex_unlock(&buffer_mutex);

    /* return value to client */
    return value;
}
