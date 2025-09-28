#define _XOPEN_SOURCE 700
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Node {
    int id;
    struct Node *next;
} Node;

static int N, M, Tcut, Tarr;

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cv = PTHREAD_COND_INITIALIZER;

static Node *q_head = NULL, *q_tail = NULL;
static int waiting = 0;
static int sleeping_barbers = 0;
static int next_client_id = 1;

static void enqueue(int id) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->id = id; n->next = NULL;
    if (!q_tail) q_head = q_tail = n;
    else { q_tail->next = n; q_tail = n; }
}
static int dequeue(void) {
    Node *n = q_head;
    int id = n->id;
    q_head = n->next;
    if (!q_head) q_tail = NULL;
    free(n);
    return id;
}

static void* barber(void *arg) {
    int b = (int)(long)arg;

    pthread_mutex_lock(&mx);
    sleeping_barbers++;
    printf("Barbeiro %d dormindo.\n", b);
    fflush(stdout);

    for (;;) {
        while (!q_head) {
            pthread_cond_wait(&cv, &mx);
        }

        int take_from_waiting = (waiting > 0);
        if (take_from_waiting) waiting--;
        int client = dequeue();

        if (sleeping_barbers > 0) sleeping_barbers--;

        printf("Barbeiro %d acordou.\n", b);
        printf("Barbeiro %d cortando o cabelo do cliente %d.\n", b, client);
        fflush(stdout);

        pthread_mutex_unlock(&mx);

        sleep(Tcut);

        pthread_mutex_lock(&mx);
        printf("Barbeiro %d terminou de cortar o cabelo do cliente %d.\n", b, client);
        fflush(stdout);

        if (!q_head) {
            sleeping_barbers++;
            printf("Barbeiro %d dormindo.\n", b);
            fflush(stdout);
        }
    }
}

static void* client_generator(void *arg) {
    (void)arg;
    for (;;) {
        pthread_mutex_lock(&mx);

        int cid = next_client_id++;
        printf("Cliente %d chegou.\n", cid);

        int can_enter = (sleeping_barbers > 0) || (waiting < M);

        if (can_enter) {
            if (sleeping_barbers == 0) waiting++;
            enqueue(cid);
            pthread_cond_signal(&cv);
        } else {
            printf("Cliente %d foi embora sem cortar o cabelo. Sala de espera cheia.\n", cid);
        }

        fflush(stdout);
        pthread_mutex_unlock(&mx);

        sleep(Tarr);
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <barbeiros> <cadeiras_espera> <tempo_corte> <intervalo_chegada>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    Tcut = atoi(argv[3]);
    Tarr = atoi(argv[4]);

    pthread_t *tb = (pthread_t*)malloc(sizeof(pthread_t)*N);
    for (int i = 0; i < N; i++)
        pthread_create(&tb[i], NULL, barber, (void*)(long)(i+1));

    pthread_t gen;
    pthread_create(&gen, NULL, client_generator, NULL);

    // Mantém o programa rodando (Ctrl+C para encerrar nos testes)
    pthread_join(gen, NULL);
    for (int i = 0; i < N; i++) pthread_join(tb[i], NULL);
    return 0;
}
