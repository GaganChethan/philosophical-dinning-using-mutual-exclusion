#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define MAX_MEALS 1

// Names and philosophies of the philosophers
const char* philosopher_names[NUM_PHILOSOPHERS] = {
    "Plato",
    "Aristotle",
    "Kant",
    "Descartes",
    "Sartre"
};

const char* philosopher_philosophies[NUM_PHILOSOPHERS] = {
    "The highest form of knowledge is self-knowledge.",
    "The roots of education are bitter, but the fruit is sweet.",
    "Act only according to that maxim by which you can at the same time will that it should become a universal law.",
    "I think, therefore I am.",
    "Man is condemned to be free; because once thrown into the world, he is responsible for everything he does."
};

// Define semaphores
sem_t forks[NUM_PHILOSOPHERS];
sem_t mutex;

// Meal counters for each philosopher
int meals_eaten[NUM_PHILOSOPHERS] = {0};

void *philosopher(void *arg) {
    int philosopher_id = *(int *)arg;
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    for (int meal_count = 0; meal_count < MAX_MEALS; meal_count++) {
        // Grab forks
        sem_wait(&mutex);
        sem_wait(&forks[left_fork]);
        sem_wait(&forks[right_fork]);
        sem_post(&mutex);

        // Eating
        printf("%s is eating. Meal count: %d\n", philosopher_names[philosopher_id], meal_count + 1);
        sleep(rand() % 3);

        // Release forks
        sem_post(&forks[left_fork]);
        sem_post(&forks[right_fork]);

        // Thinking
        printf("%s is thinking: %s\n", philosopher_names[philosopher_id], philosopher_philosophies[philosopher_id]);
        sleep(rand() % 3);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&mutex, 0, 1);

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&mutex);

    return 0;
}
