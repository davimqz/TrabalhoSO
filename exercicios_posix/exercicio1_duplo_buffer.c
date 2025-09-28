#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N1 5
#define N2 3
#define NUM_X_PROCESSES 2
#define NUM_Y_PROCESSES 2
#define NUM_Z_PROCESSES 2
#define NUM_OPERATIONS 10

int buffer_b1[N1];
int buffer_b2[N2];
int in_b1 = 0, out_b1 = 0;
int in_b2 = 0, out_b2 = 0;

sem_t empty_b1;
sem_t full_b1;
sem_t mutex_b1;

sem_t empty_b2;
sem_t full_b2;
sem_t mutex_b2;

typedef struct {
    int id;
    int operations;
} thread_data_t;

void insere(int buffer[], int *in, int item, int buffer_size, char* buffer_name) {
    buffer[*in] = item;
    printf("Item %d inserido em %s na posição %d\n", item, buffer_name, *in);
    *in = (*in + 1) % buffer_size;
}

int retira(int buffer[], int *out, int buffer_size, char* buffer_name) {
    int item = buffer[*out];
    printf("Item %d retirado de %s da posição %d\n", item, buffer_name, *out);
    *out = (*out + 1) % buffer_size;
    return item;
}

void *processo_x(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int x_id = data->id;
    int operations = data->operations;
    
    for (int i = 0; i < operations; i++) {
        int item = rand() % 100 + 1 + (x_id * 1000);
        
        printf("X%d: produzindo item %d\n", x_id, item);
        
        sem_wait(&empty_b1);
        sem_wait(&mutex_b1);
        insere(buffer_b1, &in_b1, item, N1, "B1");
        sem_post(&mutex_b1);
        sem_post(&full_b1);
        
        usleep(rand() % 200000);
    }
    
    printf("X%d: finalizando\n", x_id);
    return NULL;
}

void *processo_y(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int y_id = data->id;
    int operations = data->operations;
    
    for (int i = 0; i < operations; i++) {
        int item;
        
        sem_wait(&full_b1);
        sem_wait(&mutex_b1);
        item = retira(buffer_b1, &out_b1, N1, "B1");
        sem_post(&mutex_b1);
        sem_post(&empty_b1);
        
        printf("Y%d: processando item %d\n", y_id, item);
        usleep(rand() % 150000);
        
        int processed_item = item + 10000;
        
        sem_wait(&empty_b2);
        sem_wait(&mutex_b2);
        insere(buffer_b2, &in_b2, processed_item, N2, "B2");
        sem_post(&mutex_b2);
        sem_post(&full_b2);
    }
    
    printf("Y%d: finalizando\n", y_id);
    return NULL;
}

void *processo_z(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int z_id = data->id;
    int operations = data->operations;
    
    for (int i = 0; i < operations; i++) {
        int item;
        
        sem_wait(&full_b2);
        sem_wait(&mutex_b2);
        item = retira(buffer_b2, &out_b2, N2, "B2");
        sem_post(&mutex_b2);
        sem_post(&empty_b2);
        
        printf("Z%d: consumindo item final %d\n", z_id, item);
        usleep(rand() % 300000);
    }
    
    printf("Z%d: finalizando\n", z_id);
    return NULL;
}

int main() {
    printf("=== SISTEMA X -> B1 -> Y -> B2 -> Z ===\n");
    printf("Buffer B1: capacidade %d\n", N1);
    printf("Buffer B2: capacidade %d\n", N2);
    printf("Processos X: %d\n", NUM_X_PROCESSES);
    printf("Processos Y: %d\n", NUM_Y_PROCESSES);
    printf("Processos Z: %d\n", NUM_Z_PROCESSES);
    printf("Operações por processo: %d\n", NUM_OPERATIONS);
    printf("=======================================\n\n");
    
    sem_init(&empty_b1, 0, N1);
    sem_init(&full_b1, 0, 0);
    sem_init(&mutex_b1, 0, 1);
    
    sem_init(&empty_b2, 0, N2);
    sem_init(&full_b2, 0, 0);
    sem_init(&mutex_b2, 0, 1);
    
    srand(time(NULL));
    
    pthread_t x_threads[NUM_X_PROCESSES];
    pthread_t y_threads[NUM_Y_PROCESSES];
    pthread_t z_threads[NUM_Z_PROCESSES];
    
    thread_data_t x_data[NUM_X_PROCESSES];
    thread_data_t y_data[NUM_Y_PROCESSES];
    thread_data_t z_data[NUM_Z_PROCESSES];
    
    clock_t start_time = clock();
    
    for (int i = 0; i < NUM_X_PROCESSES; i++) {
        x_data[i].id = i + 1;
        x_data[i].operations = NUM_OPERATIONS;
        pthread_create(&x_threads[i], NULL, processo_x, &x_data[i]);
    }
    
    for (int i = 0; i < NUM_Y_PROCESSES; i++) {
        y_data[i].id = i + 1;
        y_data[i].operations = NUM_OPERATIONS;
        pthread_create(&y_threads[i], NULL, processo_y, &y_data[i]);
    }
    
    for (int i = 0; i < NUM_Z_PROCESSES; i++) {
        z_data[i].id = i + 1;
        z_data[i].operations = NUM_OPERATIONS;
        pthread_create(&z_threads[i], NULL, processo_z, &z_data[i]);
    }
    
    for (int i = 0; i < NUM_X_PROCESSES; i++) {
        pthread_join(x_threads[i], NULL);
    }
    
    for (int i = 0; i < NUM_Y_PROCESSES; i++) {
        pthread_join(y_threads[i], NULL);
    }
    
    for (int i = 0; i < NUM_Z_PROCESSES; i++) {
        pthread_join(z_threads[i], NULL);
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    printf("Tempo total: %.2f segundos\n", execution_time);
    printf("Itens produzidos: %d\n", NUM_X_PROCESSES * NUM_OPERATIONS);
    printf("Itens processados: %d\n", NUM_Y_PROCESSES * NUM_OPERATIONS);
    printf("Itens consumidos: %d\n", NUM_Z_PROCESSES * NUM_OPERATIONS);
    
    sem_destroy(&empty_b1);
    sem_destroy(&full_b1);
    sem_destroy(&mutex_b1);
    sem_destroy(&empty_b2);
    sem_destroy(&full_b2);
    sem_destroy(&mutex_b2);
    
    return 0;
}