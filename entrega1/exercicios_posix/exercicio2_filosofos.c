#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 5

sem_t garfo[5];

void filosofo_solucao1(int i) {
    while (1) {
        printf("Filósofo %d está meditando...\n", i);
        usleep(rand() % 1000000);
        
        int primeiro_garfo = (i < (i + 1) % N) ? i : (i + 1) % N;
        int segundo_garfo = (i < (i + 1) % N) ? (i + 1) % N : i;
        
        printf("Filósofo %d quer comer - pegando garfo %d\n", i, primeiro_garfo);
        sem_wait(&garfo[primeiro_garfo]);
        
        printf("Filósofo %d pegou garfo %d - pegando garfo %d\n", i, primeiro_garfo, segundo_garfo);
        sem_wait(&garfo[segundo_garfo]);
        
        printf("Filósofo %d está comendo com garfos %d e %d\n", i, primeiro_garfo, segundo_garfo);
        usleep(rand() % 500000);
        
        printf("Filósofo %d terminou de comer - liberando garfos\n", i);
        sem_post(&garfo[primeiro_garfo]);
        sem_post(&garfo[segundo_garfo]);
    }
}

sem_t mesa;

void filosofo_solucao2(int i) {
    while (1) {
        printf("Filósofo %d está meditando...\n", i);
        usleep(rand() % 1000000);
        
        sem_wait(&mesa);
        
        printf("Filósofo %d quer comer - pegando garfo %d\n", i, i);
        sem_wait(&garfo[i]);
        
        printf("Filósofo %d pegou garfo %d - pegando garfo %d\n", i, i, (i + 1) % N);
        sem_wait(&garfo[(i + 1) % N]);
        
        printf("Filósofo %d está comendo\n", i);
        usleep(rand() % 500000);
        
        printf("Filósofo %d terminou de comer - liberando garfos\n", i);
        sem_post(&garfo[i]);
        sem_post(&garfo[(i + 1) % N]);
        
        sem_post(&mesa);
    }
}

void filosofo_solucao3(int i) {
    while (1) {
        printf("Filósofo %d está meditando...\n", i);
        usleep(rand() % 1000000);
        
        if (i == N - 1) {
            printf("Filósofo %d (especial) quer comer - pegando garfo %d\n", i, (i + 1) % N);
            sem_wait(&garfo[(i + 1) % N]);
            
            printf("Filósofo %d pegou garfo %d - pegando garfo %d\n", i, (i + 1) % N, i);
            sem_wait(&garfo[i]);
            
            printf("Filósofo %d está comendo\n", i);
            usleep(rand() % 500000);
            
            printf("Filósofo %d terminou de comer - liberando garfos\n", i);
            sem_post(&garfo[(i + 1) % N]);
            sem_post(&garfo[i]);
        } else {
            printf("Filósofo %d quer comer - pegando garfo %d\n", i, i);
            sem_wait(&garfo[i]);
            
            printf("Filósofo %d pegou garfo %d - pegando garfo %d\n", i, i, (i + 1) % N);
            sem_wait(&garfo[(i + 1) % N]);
            
            printf("Filósofo %d está comendo\n", i);
            usleep(rand() % 500000);
            
            printf("Filósofo %d terminou de comer - liberando garfos\n", i);
            sem_post(&garfo[i]);
            sem_post(&garfo[(i + 1) % N]);
        }
    }
}

void *thread_filosofo(void *arg) {
    int i = *(int*)arg;
    filosofo_solucao1(i);
    return NULL;
}

int main(int argc, char *argv[]) {
    int solucao = 1;
    
    if (argc > 1) {
        solucao = atoi(argv[1]);
        if (solucao < 1 || solucao > 3) solucao = 1;
    }
    
    printf("=== JANTAR DOS FILÓSOFOS - SOLUÇÃO %d ===\n", solucao);
    switch(solucao) {
        case 1: printf("Estratégia: Ordenação de recursos\n"); break;
        case 2: printf("Estratégia: Limitação de filósofos à mesa\n"); break;
        case 3: printf("Estratégia: Filósofo especial\n"); break;
    }
    printf("Filósofos: %d\n", N);
    printf("==========================================\n\n");
    
    for (int i = 0; i < N; i++) {
        sem_init(&garfo[i], 0, 1);
    }
    
    sem_init(&mesa, 0, N - 1);
    
    srand(time(NULL));
    
    pthread_t filosofos[N];
    int ids[N];
    
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        
        if (solucao == 2) {
            pthread_create(&filosofos[i], NULL, 
                          (void*(*)(void*))filosofo_solucao2, &ids[i]);
        } else if (solucao == 3) {
            pthread_create(&filosofos[i], NULL, 
                          (void*(*)(void*))filosofo_solucao3, &ids[i]);
        } else {
            pthread_create(&filosofos[i], NULL, thread_filosofo, &ids[i]);
        }
    }
    
    printf("Pressione Ctrl+C para parar a simulação...\n\n");
    
    for (int i = 0; i < N; i++) {
        pthread_join(filosofos[i], NULL);
    }
    
    for (int i = 0; i < N; i++) {
        sem_destroy(&garfo[i]);
    }
    sem_destroy(&mesa);
    
    return 0;
}