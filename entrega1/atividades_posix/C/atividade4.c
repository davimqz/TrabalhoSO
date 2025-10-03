#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define NUM_OPERATIONS 5

typedef struct {
    int data;
    pthread_rwlock_t rwlock;
} shared_resource_t;

shared_resource_t resource;

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        pthread_rwlock_rdlock(&resource.rwlock);
        
        printf("Leitor %d: lendo dados = %d\n", reader_id, resource.data);
        usleep(rand() % 200000);
        
        pthread_rwlock_unlock(&resource.rwlock);
        
        usleep(rand() % 300000);
    }
    
    printf("Leitor %d: finalizando\n", reader_id);
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int new_data = rand() % 1000 + 1;
        
        pthread_rwlock_wrlock(&resource.rwlock);
        
        printf("Escritor %d: escrevendo dados %d -> %d\n", 
               writer_id, resource.data, new_data);
        resource.data = new_data;
        usleep(rand() % 250000);
        
        pthread_rwlock_unlock(&resource.rwlock);
        
        usleep(rand() % 400000);
    }
    
    printf("Escritor %d: finalizando\n", writer_id);
    return NULL;
}

int main(int argc, char* argv[]) {
    int num_readers = NUM_READERS;
    int num_writers = NUM_WRITERS;
    
    if (argc == 3) {
        num_readers = atoi(argv[1]);
        num_writers = atoi(argv[2]);
    }
    
    printf("=== LEITORES/ESCRITORES - RWLOCKS (PRIORIDADE ESCRITORES) ===\n");
    printf("Operações por thread: %d\n", NUM_OPERATIONS);
    printf("Leitores: %d\n", num_readers);
    printf("Escritores: %d\n", num_writers);
    printf("=============================================================\n\n");
    
    srand(time(NULL));
    
    resource.data = 42;
    
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NP);
    pthread_rwlock_init(&resource.rwlock, &attr);
    pthread_rwlockattr_destroy(&attr);
    
    pthread_t* reader_threads = malloc(num_readers * sizeof(pthread_t));
    pthread_t* writer_threads = malloc(num_writers * sizeof(pthread_t));
    int* reader_ids = malloc(num_readers * sizeof(int));
    int* writer_ids = malloc(num_writers * sizeof(int));
    
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]);
    }
    
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
    }
    
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writer_threads[i], NULL);
    }
    
    for (int i = 0; i < num_readers; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    printf("Valor final dos dados: %d\n", resource.data);
    
    pthread_rwlock_destroy(&resource.rwlock);
    
    free(reader_threads);
    free(writer_threads);
    free(reader_ids);
    free(writer_ids);
    
    return 0;
}