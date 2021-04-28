#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/semaphore.h"
#include "../include/shmem.h"
#include "../include/utilities.h"
#include "../include/structs.h"
#include "../include/print.h"

double waiting_time;
double blocked_time;
struct timeval tic, toc, tic2, toc2, tic_kernel, toc_kernel;
double result, result_kernel;
int counter;
int memory_index = 0;

double ranExpo(double lambda);
void writeMsg(int data1, int data2, char *data3, int index,
               struct sembuf operation, int id, message *memory,
               globalVars *memory2, int buffer_size);

int main(int argc, char *argv[]) {
  /* Argument validation */

  char *buffer_name = argv[1];
  float seconds = atof(argv[2]);

  if (argc != 3) {
    printf("Usage: ./producer <buffer_name> <time_medium>\n");
    exit(0);
  }

  if (seconds == 0 || !isFloat(argv[2])) {
    printf("Please insert a correct time medium size\n");
    exit(0);
  }

  srand((unsigned)time(NULL));

  /* Shared memory for global variables initializartion */
  key_t key_memory;
  int id_memory;
  message *memory = NULL;

  key_t key_global;
  int gv_shm_id;
  globalVars *memory2 = NULL;

  if (checkBinDir()) {
    key_global = ftok("../share_files/global", 33);
  } else {
    key_global = ftok("share_files/global", 33);
  }

  if (key_global == -1) {
    printf("Global Memory Key is Invalid\n");
    exit(0);
  }

  if (getGlobal(&gv_shm_id, key_global) == 0) {
    printf("Can't get global id\n");
    exit(0);
  }

  if (getGlobalMemory(&gv_shm_id, &memory2) == 0) {
    printf("Can't get global memory\n");
    exit(0);
  }

  /* Gets buffer size and increments producers counter */
  int buffer_size = memory2->size;

  /* Shared memory for buffer initializartion */
  char *key_route;
  if (checkBinDir()) {
    key_route = concat("../share_files/", buffer_name);
  } else {
    key_route = concat("share_files/", buffer_name);
  }

  key_memory = ftok(key_route, 33);
  if (key_memory == -1) {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (getBuffer(&id_memory, key_memory, buffer_size) == 0) {
    printf("Can't get buffer id\n");
    exit(0);
  }

  if (getBufferMemory(&id_memory, &memory) == 0) {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Create semaphores */
  int id_semaphore;
  if (checkBinDir()) {
    id_semaphore = initSemaphore("../share_files/sem", buffer_size);
  } else {
    id_semaphore = initSemaphore("share_files/sem", buffer_size);
  }

  struct sembuf operation;
  operation.sem_flg = 0;
  int current_pid = getpid();

  /* Increments producers counter */
  operation.sem_num = buffer_size;
  operation.sem_op = -1;
  semop(id_semaphore, &operation, 1);

  memory2->producers++;
  memory2->total_producers++;

  operation.sem_op = 1;
  semop(id_semaphore, &operation, 1);

  while (1) {
    /* Exponential Distribution waiting  */
    float s = ranExpo(seconds);
    waiting_time += s;

    char waiting_print[30];
    sprintf(waiting_print, "Waiting time %f\n", s);
    printColor(waiting_print, 1);

    sleep(s);

    /* Down for producer semaphore */
    operation.sem_num = buffer_size + 1;
    operation.sem_op = -1;

    /* Get blocked time producers semaphore */
    gettimeofday(&tic, NULL);

    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc, NULL);
    result = (double)(toc.tv_usec - tic.tv_usec) / 1000000 +
             (double)(toc.tv_sec - tic.tv_sec);

    blocked_time += result;

    /* Writes to global memory */
    operation.sem_num = buffer_size;
    operation.sem_op = -1;
    semop(id_semaphore, &operation, 1);

    /* Check for finish flag */
    if (memory2->kill) {

      /* Writes stadistics on global varibles */
      memory2->producers--;
      memory2->waiting_time += waiting_time;
      memory2->blocked_time += blocked_time;
      memory2->kernel_time += (waiting_time + blocked_time);
      operation.sem_op = 1;
      semop(id_semaphore, &operation, 1);

      printProducerEnd(current_pid, counter, waiting_time, blocked_time,
                         (waiting_time + blocked_time));

      exit(0);
    }

    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);

    /* Get next index to write */
    int index = getIndex(0, buffer_size, memory, memory_index, id_semaphore);
    memory_index = index + 1;

    /* Write Operation, Down to semaphore */
    operation.sem_num = index;
    operation.sem_op = -1;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[64];
    strftime(date, sizeof(date), "%c", tm);

    gettimeofday(&tic2, NULL);

    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc2, NULL);
    result = (double)(toc2.tv_usec - tic2.tv_usec) / 1000000 +
             (double)(toc2.tv_sec - tic2.tv_sec);

    blocked_time += result;

    /* Write on shared memory */
    writeMsg(current_pid, rand() % 6, date, index, operation, id_semaphore,
              memory, memory2, buffer_size);

    /* Up to semaphore */
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);

    counter++;

    /* Up for consumer semaphore */
    operation.sem_num = buffer_size + 2;
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);
  }

  return 0;
}

/* Exponential Distribution */
// **param lambda: seconds double
double ranExpo(double lambda) {
  double u;
  u = rand() / (RAND_MAX + 1.0);
  return -log(1 - u) / lambda;
}

/* Write on shared memory */
void writeMsg(int data1, int data2, char *data3, int index,
               struct sembuf operation, int id, message *memory,
               globalVars *memory2, int buffer_size) {

  /* Memory Write */
  memory[index].pid = data1;
  memory[index].magic_number = data2;
  memory[index].is_used = 1;
  strcpy(memory[index].date, data3);

  operation.sem_num = buffer_size;
  operation.sem_op = -1;
  semop(id, &operation, 1);

  int consumers = memory2->consumers;
  int producers = memory2->producers;
  memory2->messages++;

  operation.sem_op = 1;
  semop(id, &operation, 1);

  printProducerMessage(index, consumers, producers);
}
