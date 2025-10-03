#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t vez_bart;
sem_t vez_lisa;
sem_t vez_maggie;

int movimento_atual = 1;
pthread_mutex_t mutex_contador = PTHREAD_MUTEX_INITIALIZER;

void move(char* nome, int id) {
    printf("Movimento %d: Robô %s se movendo...\n", movimento_atual, nome);
    usleep(500000 + rand() % 500000);
    printf("Movimento %d: Robô %s completou movimento\n", movimento_atual, nome);
}

void *robo_bart(void *arg) {
    int bart_id = *(int*)arg;
    
    while (1) {
        sem_wait(&vez_bart);
        
        move("Bart", bart_id);
        
        pthread_mutex_lock(&mutex_contador);
        movimento_atual++;
        
        printf("Bart liberando para Lisa (movimento %d)\n", movimento_atual);
        sem_post(&vez_lisa);
        
        pthread_mutex_unlock(&mutex_contador);
    }
    
    return NULL;
}

void *robo_lisa(void *arg) {
    int lisa_id = *(int*)arg;
    
    while (1) {
        sem_wait(&vez_lisa);
        
        move("Lisa", lisa_id);
        
        pthread_mutex_lock(&mutex_contador);
        movimento_atual++;
        
        if (movimento_atual % 4 == 3) {
            printf("Lisa liberando para Maggie (movimento %d)\n", movimento_atual);
            sem_post(&vez_maggie);
        } else {
            printf("Lisa liberando para Bart (movimento %d)\n", movimento_atual);
            sem_post(&vez_bart);
        }
        
        pthread_mutex_unlock(&mutex_contador);
    }
    
    return NULL;
}

void *robo_maggie(void *arg) {
    int maggie_id = *(int*)arg;
    
    while (1) {
        sem_wait(&vez_maggie);
        
        move("Maggie", maggie_id);
        
        pthread_mutex_lock(&mutex_contador);
        movimento_atual++;
        
        printf("Maggie liberando para Lisa (movimento %d)\n", movimento_atual);
        sem_post(&vez_lisa);
        
        pthread_mutex_unlock(&mutex_contador);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    int max_movimentos = 20;
    
    if (argc > 1) {
        max_movimentos = atoi(argv[1]);
        if (max_movimentos < 1) max_movimentos = 20;
    }
    
    printf("=== SINCRONIZAÇÃO DE ROBÔS ===\n");
    printf("Sequência: Bart → Lisa → Maggie → Lisa → Bart → Lisa → Maggie → ...\n");
    printf("Máximo de movimentos para demonstração: %d\n", max_movimentos);
    printf("===============================\n\n");
    
    sem_init(&vez_bart, 0, 1);
    sem_init(&vez_lisa, 0, 0);
    sem_init(&vez_maggie, 0, 0);
    
    srand(time(NULL));
    
    pthread_t thread_bart, thread_lisa, thread_maggie;
    int id_bart = 1, id_lisa = 2, id_maggie = 3;
    
    pthread_create(&thread_bart, NULL, robo_bart, &id_bart);
    pthread_create(&thread_lisa, NULL, robo_lisa, &id_lisa);  
    pthread_create(&thread_maggie, NULL, robo_maggie, &id_maggie);
    
    sleep(max_movimentos);
    
    printf("\n=== SIMULAÇÃO FINALIZADA ===\n");
    printf("Total de movimentos demonstrados: %d\n", movimento_atual - 1);
    
    pthread_cancel(thread_bart);
    pthread_cancel(thread_lisa);
    pthread_cancel(thread_maggie);
    
    pthread_join(thread_bart, NULL);
    pthread_join(thread_lisa, NULL);
    pthread_join(thread_maggie, NULL);
    
    sem_destroy(&vez_bart);
    sem_destroy(&vez_lisa);
    sem_destroy(&vez_maggie);
    pthread_mutex_destroy(&mutex_contador);
    
    return 0;
}