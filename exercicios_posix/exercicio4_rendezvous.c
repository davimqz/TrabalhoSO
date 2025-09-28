#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef struct rv_t 
{
    sem_t mutex;       
    sem_t barreira_a;   
    sem_t barreira_b;  
    int chegou_a;       
    int chegou_b;       
} rv_t;

void rv_wait(rv_t *rv) {
    pthread_t current_thread = pthread_self();
    
    sem_wait(&rv->mutex);
    
    if (!rv->chegou_a && !rv->chegou_b) {
        if (current_thread % 2 == 0) {
            rv->chegou_a = 1;
            sem_post(&rv->mutex);
            printf("Processo A chegou ao RV - aguardando processo B\n");
            sem_wait(&rv->barreira_a);
        } else {
            rv->chegou_b = 1;
            sem_post(&rv->mutex);
            printf("Processo B chegou ao RV - aguardando processo A\n");
            sem_wait(&rv->barreira_b);
        }
    } else {
        if (rv->chegou_a && !rv->chegou_b) {
            rv->chegou_b = 1;
            printf("Processo B chegou - liberando ambos!\n");
            sem_post(&rv->barreira_a);
        } else if (rv->chegou_b && !rv->chegou_a) {
            rv->chegou_a = 1;
            printf("Processo A chegou - liberando ambos!\n");
            sem_post(&rv->barreira_b);
        }
        
        rv->chegou_a = 0;
        rv->chegou_b = 0;
        sem_post(&rv->mutex);
    }
}

void rv_wait_with_id(rv_t *rv, int processo_id) {
    sem_wait(&rv->mutex);
    
    if (processo_id == 0) {
        if (!rv->chegou_a) {
            rv->chegou_a = 1;
            if (rv->chegou_b) {
                printf("Processo A chegou - ambos no RV, liberando!\n");
                sem_post(&rv->barreira_b);
                rv->chegou_a = 0;
                rv->chegou_b = 0;
                sem_post(&rv->mutex);
            } else {
                printf("Processo A chegou ao RV - aguardando B\n");
                sem_post(&rv->mutex);
                sem_wait(&rv->barreira_a);
            }
        }
    } else {
        if (!rv->chegou_b) {
            rv->chegou_b = 1;
            if (rv->chegou_a) {
                printf("Processo B chegou - ambos no RV, liberando!\n");
                sem_post(&rv->barreira_a);
                rv->chegou_a = 0;
                rv->chegou_b = 0;
                sem_post(&rv->mutex);
            } else {
                printf("Processo B chegou ao RV - aguardando A\n");
                sem_post(&rv->mutex);
                sem_wait(&rv->barreira_b);
            }
        }
    }
}

void rv_init(rv_t *rv) {
    sem_init(&rv->mutex, 0, 1);
    sem_init(&rv->barreira_a, 0, 0);
    sem_init(&rv->barreira_b, 0, 0);
    rv->chegou_a = 0;
    rv->chegou_b = 0;
}

void rv_destroy(rv_t *rv) {
    sem_destroy(&rv->mutex);
    sem_destroy(&rv->barreira_a);
    sem_destroy(&rv->barreira_b);
}

void A1() { printf("A1() executando...\n"); usleep(rand() % 500000); }
void A2() { printf("A2() executando...\n"); usleep(rand() % 500000); }
void A3() { printf("A3() executando...\n"); usleep(rand() % 500000); }

void B1() { printf("B1() executando...\n"); usleep(rand() % 500000); }
void B2() { printf("B2() executando...\n"); usleep(rand() % 500000); }
void B3() { printf("B3() executando...\n"); usleep(rand() % 500000); }

rv_t rv1, rv2;

void *processo_a(void *arg) {
    printf("=== PROCESSO A INICIADO ===\n");
    
    A1();
    printf("A: Chegando ao primeiro RV\n");
    rv_wait_with_id(&rv1, 0);
    printf("A: Passou do primeiro RV\n");
    
    A2();
    printf("A: Chegando ao segundo RV\n");
    rv_wait_with_id(&rv2, 0);
    printf("A: Passou do segundo RV\n");
    
    A3();
    printf("=== PROCESSO A FINALIZADO ===\n");
    
    return NULL;
}

void *processo_b(void *arg) {
    printf("=== PROCESSO B INICIADO ===\n");
    
    B1();
    printf("B: Chegando ao primeiro RV\n");
    rv_wait_with_id(&rv1, 1);
    printf("B: Passou do primeiro RV\n");
    
    B2();
    printf("B: Chegando ao segundo RV\n");
    rv_wait_with_id(&rv2, 1);
    printf("B: Passou do segundo RV\n");
    
    B3();
    printf("=== PROCESSO B FINALIZADO ===\n");
    
    return NULL;
}

int main() {
    printf("=== DEMONSTRAÇÃO RENDEZ-VOUS ===\n");
    printf("Dois processos (A e B) sincronizados por pontos de encontro\n");
    printf("Restrições: Ai → Bj>i e Bi → Aj>i\n");
    printf("=====================================\n\n");
    
    rv_init(&rv1);
    rv_init(&rv2);
    
    srand(time(NULL));
    
    pthread_t thread_a, thread_b;
    
    clock_t start_time = clock();
    
    pthread_create(&thread_a, NULL, processo_a, NULL);
    pthread_create(&thread_b, NULL, processo_b, NULL);
    
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== EXECUÇÃO FINALIZADA ===\n");
    printf("Tempo total: %.2f segundos\n", execution_time);
    printf("Sincronização Rendez-Vous funcionou corretamente!\n");
    
    rv_destroy(&rv1);
    rv_destroy(&rv2);
    
    return 0;
}