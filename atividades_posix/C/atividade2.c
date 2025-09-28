#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS 20

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} shared_buffer_t;

shared_buffer_t shared;

void* producer(void* arg) {
    int producer_id = *(int*)arg;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100 + 1;
        
        pthread_mutex_lock(&shared.mutex);
        
        while (shared.count == BUFFER_SIZE) {
            pthread_cond_wait(&shared.not_full, &shared.mutex);
        }
        
        shared.buffer[shared.in] = item;
        printf("Produtor %d: produzido item %d na posição %d (count=%d)\n", 
               producer_id, item, shared.in, shared.count + 1);
        shared.in = (shared.in + 1) % BUFFER_SIZE;
        shared.count++;
        
        pthread_cond_signal(&shared.not_empty);
        pthread_mutex_unlock(&shared.mutex);
        
        usleep(rand() % 100000);
    }
    
    printf("Produtor %d: finalizando\n", producer_id);
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&shared.mutex);
        
        while (shared.count == 0) {
            pthread_cond_wait(&shared.not_empty, &shared.mutex);
        }
        
        int item = shared.buffer[shared.out];
        printf("Consumidor %d: consumido item %d da posição %d (count=%d)\n", 
               consumer_id, item, shared.out, shared.count - 1);
        shared.out = (shared.out + 1) % BUFFER_SIZE;
        shared.count--;
        
        pthread_cond_signal(&shared.not_full);
        pthread_mutex_unlock(&shared.mutex);
        
        usleep(rand() % 150000);
    }
    
    printf("Consumidor %d: finalizando\n", consumer_id);
    return NULL;
}

int main(int argc, char* argv[]) {
    int num_producers = 2;
    int num_consumers = 2;
    
    if (argc == 3) {
        num_producers = atoi(argv[1]);
        num_consumers = atoi(argv[2]);
    }
    
    printf("=== PRODUTOR/CONSUMIDOR - VARIÁVEIS DE CONDIÇÃO ===\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Items per thread: %d\n", NUM_ITEMS);
    printf("Produtores: %d\n", num_producers);
    printf("Consumidores: %d\n", num_consumers);
    printf("==================================================\n\n");
    
    srand(time(NULL));
    
    shared.count = 0;
    shared.in = 0;
    shared.out = 0;
    
    pthread_mutex_init(&shared.mutex, NULL);
    pthread_cond_init(&shared.not_full, NULL);
    pthread_cond_init(&shared.not_empty, NULL);
    
    pthread_t* producer_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t* consumer_threads = malloc(num_consumers * sizeof(pthread_t));
    int* producer_ids = malloc(num_producers * sizeof(int));
    int* consumer_ids = malloc(num_consumers * sizeof(int));
    
    for (int i = 0; i < num_producers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }
    
    for (int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }
    
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }
    
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    
    pthread_mutex_destroy(&shared.mutex);
    pthread_cond_destroy(&shared.not_full);
    pthread_cond_destroy(&shared.not_empty);
    
    free(producer_threads);
    free(consumer_threads);
    free(producer_ids);
    free(consumer_ids);
    
    return 0;
}