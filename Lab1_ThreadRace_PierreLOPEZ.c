// Pierre LOPEZ Classe 40 internationale
// lab1 thread

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Nombre total de thread
#define NB_THREAD 2
#define INCREMENT_LIMIT 200000
pthread_t threads[NB_THREAD];
int Global_counter = 0;

// Structure de données contenant le mutex
typedef struct mutex_data {
  int step;
  pthread_mutex_t mutex;
} mutex_data;

// Fonction exécutée dans le thread
void *runner(void *arg) {
  mutex_data *md = (mutex_data *)arg;
  pthread_t tid = pthread_self();
  while ((*md).step < INCREMENT_LIMIT) {
    // Vérouillage du mutex
    pthread_mutex_lock(&(*md).mutex);
    (*md).step++;
    Global_counter++;
    // Dévérouillage du mutex
    pthread_mutex_unlock(&(*md).mutex);
  }
  // printf("\n Step1 = %i \n", (*md).data);
  printf("Fin du thread %ld\n", tid);
  pthread_exit(NULL);
}
int main() {
  // Création du mutex
  mutex_data md1;
  mutex_data md2;
  md1.step = 0;
  md2.step = 0;

  // Initialisation du mutex
  if (pthread_mutex_init(&md1.mutex, NULL) != 0 ||
      pthread_mutex_init(&md2.mutex, NULL) != 0) {
    printf("\n mutex init failed\n");
    return EXIT_FAILURE;
  }
  int err;
  err = pthread_create(&threads[0], NULL, runner, &md1);
  if (err != 0) {
    printf("Echec de la création du thread: [%s]", strerror(err));
  }
  err = pthread_create(&threads[1], NULL, runner, &md2);
  if (err != 0) {
    printf("Echec de la création du thread: [%s]", strerror(err));
  }

  // En attente des threads
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  // Destruction des mutex
  pthread_mutex_destroy(&md1.mutex);
  pthread_mutex_destroy(&md2.mutex);

  printf("\nStep1 = %i \n", md1.step);
  printf("Step2 = %i \n", md2.step);
  printf("Sum of Global_counter = %i\n", Global_counter);
  printf("Sum of Step1 + Step2 = %i\n", md1.step + md2.step);
  return EXIT_SUCCESS;
}