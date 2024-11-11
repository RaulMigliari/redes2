#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t barrier_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;
int count = 0;
int num_threads = 3;

void *barrier_thread(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d reached the barrier\n", id);

    pthread_mutex_lock(&barrier_mutex);
    count++;
    if (count == num_threads) {
        printf("All threads reached the barrier, releasing...\n");
        pthread_cond_broadcast(&barrier_cond);
    } else {
        pthread_cond_wait(&barrier_cond, &barrier_mutex);
    }
    pthread_mutex_unlock(&barrier_mutex);

    printf("Thread %d passed the barrier\n", id);
    return NULL;
}

int main() {
    pthread_t threads[num_threads];
    int ids[num_threads];

    for (int i = 0; i < num_threads; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, barrier_thread, &ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
