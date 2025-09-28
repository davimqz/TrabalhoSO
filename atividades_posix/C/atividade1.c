#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10
#define NUM_OPERATIONS 10

// Buffer compartilhado e suas variáveis de controle
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

// Semáforos para sincronização
sem_t empty;  // Conta posições vazias
sem_t full;   // Conta posições ocupadas
sem_t mutex;  // Exclusão mútua para o buffer

// Estrutura para passar dados para as threads
typedef struct {
    int id;
    int num_operations;
} thread_data_t;

void *producer(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int producer_id = data->id;
    int operations = data->num_operations;
    
    for (int i = 0; i < operations; i++) {
        int item = rand() % 100 + 1; // Item entre 1 e 100
        
        // Esperar por slot vazio
        sem_wait(&empty);
        
        // Entrar na seção crítica
        sem_wait(&mutex);
        
        // Adicionar item ao buffer
        buffer[in] = item;
        printf("Produtor %d: produziu item %d na posição %d\n", 
               producer_id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        
        // Sair da seção crítica
        sem_post(&mutex);
        
        // Sinalizar que há um item disponível
        sem_post(&full);
        
        // Simular tempo de produção
        usleep(rand() % 200000); // 0-200ms
    }
    
    printf("Produtor %d: finalizando\n", producer_id);
    return NULL;
}

void *consumer(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int consumer_id = data->id;
    int operations = data->num_operations;
    
    for (int i = 0; i < operations; i++) {
        // Esperar por item disponível
        sem_wait(&full);
        
        // Entrar na seção crítica
        sem_wait(&mutex);
        
        // Remover item do buffer
        int item = buffer[out];
        printf("Consumidor %d: consumiu item %d da posição %d\n", 
               consumer_id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        
        // Sair da seção crítica
        sem_post(&mutex);
        
        // Sinalizar que há um slot vazio
        sem_post(&empty);
        
        // Simular tempo de consumo
        usleep(rand() % 300000); // 0-300ms
    }
    
    printf("Consumidor %d: finalizando\n", consumer_id);
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_producers = 2;
    int num_consumers = 2;
    
    // Permitir configuração via argumentos
    if (argc == 3) {
        num_producers = atoi(argv[1]);
        num_consumers = atoi(argv[2]);
    }
    
    printf("=== PRODUTOR/CONSUMIDOR COM SEMÁFOROS ===\n");
    printf("Tamanho do buffer: %d\n", BUFFER_SIZE);
    printf("Operações por thread: %d\n", NUM_OPERATIONS);
    printf("Produtores: %d\n", num_producers);
    printf("Consumidores: %d\n", num_consumers);
    printf("==========================================\n\n");
    
    // Inicializar semáforos
    if (sem_init(&empty, 0, BUFFER_SIZE) != 0) {
        perror("Erro ao inicializar semáforo empty");
        exit(1);
    }
    
    if (sem_init(&full, 0, 0) != 0) {
        perror("Erro ao inicializar semáforo full");
        exit(1);
    }
    
    if (sem_init(&mutex, 0, 1) != 0) {
        perror("Erro ao inicializar semáforo mutex");
        exit(1);
    }
    
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    // Criar arrays de threads e dados
    pthread_t *producer_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t *consumer_threads = malloc(num_consumers * sizeof(pthread_t));
    thread_data_t *producer_data = malloc(num_producers * sizeof(thread_data_t));
    thread_data_t *consumer_data = malloc(num_consumers * sizeof(thread_data_t));
    
    clock_t start_time = clock();
    
    // Criar threads produtoras
    for (int i = 0; i < num_producers; i++) {
        producer_data[i].id = i + 1;
        producer_data[i].num_operations = NUM_OPERATIONS;
        
        if (pthread_create(&producer_threads[i], NULL, producer, &producer_data[i]) != 0) {
            perror("Erro ao criar thread produtora");
            exit(1);
        }
    }
    
    // Criar threads consumidoras
    for (int i = 0; i < num_consumers; i++) {
        consumer_data[i].id = i + 1;
        consumer_data[i].num_operations = NUM_OPERATIONS;
        
        if (pthread_create(&consumer_threads[i], NULL, consumer, &consumer_data[i]) != 0) {
            perror("Erro ao criar thread consumidora");
            exit(1);
        }
    }
    
    // Aguardar finalização das threads produtoras
    for (int i = 0; i < num_producers; i++) {
        if (pthread_join(producer_threads[i], NULL) != 0) {
            perror("Erro ao aguardar thread produtora");
            exit(1);
        }
    }
    
    // Aguardar finalização das threads consumidoras
    for (int i = 0; i < num_consumers; i++) {
        if (pthread_join(consumer_threads[i], NULL) != 0) {
            perror("Erro ao aguardar thread consumidora");
            exit(1);
        }
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    printf("Tempo total: %.2f segundos\n", execution_time);
    printf("Total de itens produzidos: %d\n", num_producers * NUM_OPERATIONS);
    printf("Total de itens consumidos: %d\n", num_consumers * NUM_OPERATIONS);
    
    // Limpar recursos
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    free(producer_threads);
    free(consumer_threads);
    free(producer_data);
    free(consumer_data);
    
    return 0;
}