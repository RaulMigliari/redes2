#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
int readers = 0;
int shared_data = 0;

void *reader(void *arg) {
    pthread_mutex_lock(&resource_mutex);
    readers++;
    if (readers == 1) {
        pthread_cond_wait(&cond_var, &resource_mutex);
    }
    pthread_mutex_unlock(&resource_mutex);

    // Leitura do recurso
    printf("Reader %d: read data = %d\n", *(int *)arg, shared_data);

    pthread_mutex_lock(&resource_mutex);
    readers--;
    if (readers == 0) {
        pthread_cond_signal(&cond_var);
    }
    pthread_mutex_unlock(&resource_mutex);
    return NULL;
}

void *writer(void *arg) {
    pthread_mutex_lock(&resource_mutex);
    pthread_cond_wait(&cond_var, &resource_mutex);
    
    // Escrita no recurso
    shared_data++;
    printf("Writer %d: wrote data = %d\n", *(int *)arg, shared_data);

    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&resource_mutex);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    int ids[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }
    return 0;
}
