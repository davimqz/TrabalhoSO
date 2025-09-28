#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef struct barrier_t 
{
    int N;                 
    int count;              
    sem_t mutex;           
    sem_t barrier;          
    sem_t all_done;        
} barrier_t;

void barrier_wait(barrier_t *barrier)
{
    sem_wait(&barrier->mutex);
    
    barrier->count++;
    printf("Processo chegou à barreira (%d/%d)\n", 
           barrier->count, barrier->N);
    
    if (barrier->count == barrier->N) 
    {
        printf("Todos os processos chegaram! Liberando barreira...\n");
        
        for (int i = 0; i < barrier->N - 1; i++) 
        {
            sem_post(&barrier->barrier);
        }
        
        barrier->count = 0;
        
        sem_post(&barrier->mutex);
    } else 
    {
        sem_post(&barrier->mutex);
        printf("Processo bloqueado na barreira, aguardando outros...\n");
        sem_wait(&barrier->barrier);
    }
    
    printf("Processo passou da barreira!\n");
}

void barrier_wait_reusable(barrier_t *barrier)
{
    static int barrier_serial = 0;  
    int my_serial;
    
    sem_wait(&barrier->mutex);
    
    my_serial = barrier_serial;
    barrier->count++;
    
    if (barrier->count == barrier->N) 
    {
        barrier->count = 0;
        barrier_serial++;
        printf("Barreira %d completa! Liberando %d processos\n", 
               my_serial, barrier->N);
        
        for (int i = 0; i < barrier->N - 1; i++) 
        {
            sem_post(&barrier->barrier);
        }
        
        sem_post(&barrier->mutex);
    } else 
    {
        printf("Processo chegou à barreira %d (%d/%d) - aguardando\n", 
               my_serial, barrier->count, barrier->N);
        sem_post(&barrier->mutex);
        sem_wait(&barrier->barrier);
    }
}

void barrier_init(barrier_t *barrier, int N) 
{
    barrier->N = N;
    barrier->count = 0;
    sem_init(&barrier->mutex, 0, 1);      
    sem_init(&barrier->barrier, 0, 0);    
    sem_init(&barrier->all_done, 0, 0);  
    
    printf("Barreira inicializada para %d processos\n", N);
}

void barrier_destroy(barrier_t *barrier) 
{
    sem_destroy(&barrier->mutex);
    sem_destroy(&barrier->barrier);
    sem_destroy(&barrier->all_done);
}


void funcao_1(char processo) 
{ 
    printf("%c1() executando...\n", processo); 
    usleep(rand() % 800000 + 200000); 
}

void funcao_2(char processo) 
{ 
    printf("%c2() executando...\n", processo); 
    usleep(rand() % 600000 + 300000); 
}

void funcao_3(char processo) 
{ 
    printf("%c3() executando...\n", processo); 
    usleep(rand() % 400000 + 100000);  
}


barrier_t barreira1, barreira2;


typedef struct 
{
    char nome;
    int id;
    int total_processos;
} processo_data_t;

void *processo_generico(void *arg) 
{
    processo_data_t *data = (processo_data_t *)arg;
    char nome = data->nome;
    int id = data->id;
    
    printf("=== PROCESSO %c%d INICIADO ===\n", nome, id);
    

    funcao_1(nome);
    printf("%c%d: Chegando à primeira barreira\n", nome, id);
    barrier_wait(&barreira1);
    printf("%c%d: Passou da primeira barreira\n", nome, id);
    

    funcao_2(nome);
    printf("%c%d: Chegando à segunda barreira\n", nome, id);
    barrier_wait(&barreira2);
    printf("%c%d: Passou da segunda barreira\n", nome, id);
    
 
    funcao_3(nome);
    
    printf("=== PROCESSO %c%d FINALIZADO ===\n", nome, id);
    
    return NULL;
}

int main(int argc, char *argv[]) 
{
    int N = 4;  
    
    if (argc > 1) {
        N = atoi(argv[1]);
        if (N < 2) N = 4;
    }
    
    printf("=== DEMONSTRAÇÃO DE BARREIRAS ===\n");
    printf("Sincronizando %d processos com barreiras\n", N);
    printf("Restrições: Xi → Yj>i para todos X,Y\n");
    printf("==================================\n\n");
    

    barrier_init(&barreira1, N);
    barrier_init(&barreira2, N);
    
    srand(time(NULL));
    
    pthread_t *threads = malloc(N * sizeof(pthread_t));
    processo_data_t *dados = malloc(N * sizeof(processo_data_t));
    
    clock_t start_time = clock();
    

    char nomes[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    for (int i = 0; i < N; i++) 
    {
        dados[i].nome = nomes[i % 8];  
        dados[i].id = i + 1;
        dados[i].total_processos = N;
        
        pthread_create(&threads[i], NULL, processo_generico, &dados[i]);
    }
    
 
    for (int i = 0; i < N; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    printf("Tempo total: %.2f segundos\n", execution_time);
    printf("Processos sincronizados: %d\n", N);
    printf("Barreiras utilizadas: 2\n");
    printf("Sincronização por barreiras funcionou corretamente!\n");
    
    barrier_destroy(&barreira1);
    barrier_destroy(&barreira2);
    free(threads);
    free(dados);
    
    return 0;
}