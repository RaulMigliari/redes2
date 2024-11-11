#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

sem_t semaphore;

void *thread_func(void *arg) {
    int id = *(int *)arg;
    sem_wait(&semaphore);

    printf("Thread %d is accessing the shared resource\n", id);
    sleep(1);  // Simula acesso ao recurso compartilhado
    printf("Thread %d has finished using the shared resource\n", id);

    sem_post(&semaphore);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&semaphore, 0, 2);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    return 0;
}
