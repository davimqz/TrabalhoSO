#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define VECTOR_SIZE 100000000ULL

typedef struct { 
    const float *x; 
    size_t ini; 
    size_t fim; 
    double soma; 
} ThreadArgs;

static inline double f_function(double x) {
    double normalized = (x - 0.1) / 0.9;
    double exponential = pow(2.0, -2.0 * normalized * normalized);
    double sine_term = sin(5.0 * M_PI * x);
    return exponential * pow(sine_term, 6.0);
}

static void* worker_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    double sum = 0.0;
    
    for (size_t i = args->ini; i < args->fim; ++i) {
        sum += f_function((double)args->x[i]);
    }
    
    args->soma = sum;
    return NULL;
}

static double get_monotonic_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_threads> [tamanho_vetor]\n", argv[0]);
        return 1;
    }
    
    int num_threads = atoi(argv[1]);
    if (num_threads < 1) {
        fprintf(stderr, "Erro: número de threads deve ser >= 1\n");
        return 1;
    }
    
    size_t vector_size = (argc >= 3) ? strtoull(argv[2], NULL, 10) : VECTOR_SIZE;
    if (vector_size == 0) {
        fprintf(stderr, "Erro: tamanho do vetor deve ser > 0\n");
        return 1;
    }
    
    float* x = (float*) malloc(vector_size * sizeof(float));
    if (!x) {
        fprintf(stderr, "Erro: memória insuficiente para %zu elementos\n", vector_size);
        return 1;
    }
    
    srand(12345);
    for (size_t i = 0; i < vector_size; ++i) {
        x[i] = (float) rand() / (float) RAND_MAX;
    }
    
    double start_time = get_monotonic_time();
    
    pthread_t* threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* thread_args = (ThreadArgs*) malloc(num_threads * sizeof(ThreadArgs));
    
    if (!threads || !thread_args) {
        fprintf(stderr, "Erro: não foi possível alocar memória para threads\n");
        free(x);
        free(threads);
        free(thread_args);
        return 1;
    }
    
    size_t block_size = vector_size / (size_t)num_threads;
    size_t start_idx = 0;
    
    for (int k = 0; k < num_threads; ++k) {
        size_t end_idx = (k == num_threads - 1) ? vector_size : start_idx + block_size;
        
        thread_args[k].x = x;
        thread_args[k].ini = start_idx;
        thread_args[k].fim = end_idx;
        thread_args[k].soma = 0.0;
        
        if (pthread_create(&threads[k], NULL, worker_thread, &thread_args[k]) != 0) {
            fprintf(stderr, "Erro: não foi possível criar thread %d\n", k);
            free(x);
            free(threads);
            free(thread_args);
            return 1;
        }
        
        start_idx = end_idx;
    }
    
    double total_sum = 0.0;
    for (int k = 0; k < num_threads; ++k) {
        pthread_join(threads[k], NULL);
        total_sum += thread_args[k].soma;
    }
    
    double end_time = get_monotonic_time();
    double execution_time = end_time - start_time;
    double average = total_sum / (double)vector_size;
    
    printf("threads=%d elementos=%zu media=%.12f tempo=%.3f s\n", 
           num_threads, vector_size, average, execution_time);
    
    free(thread_args);
    free(threads);
    free(x);
    
    return 0;
}
