#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include "../include/semaphore.h"
#include "../include/shmem.h"
#include "../include/utilities.h"
#include "../include/structs.h"
#include "../include/print.h"

int main(int argc, char *argv[]) {
  /* Argument Validation */
  if (argc != 2) {
    printf("Usage: ./finalizer <buffer_name>\n");
    exit(0);
  }

  char *buffer_name = argv[1];

  /* Shared Memory for Global Variables Initialization */
  int gv_shm_id;
  key_t key_global;
  globalVars *memory2;

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
    printf("Can't get global memory to finish it\n");
    exit(0);
  }

  if (getGlobalMemory(&gv_shm_id, &memory2) == 0) {
    printf("Can't get global memory\n");
    exit(0);
  }

  int buffer_size = memory2->size;

  /* Check running directory */
  char *key_route;
  if (checkBinDir()) {
    key_route = concat("../share_files/", buffer_name);
  } else {
    key_route = concat("share_files/", buffer_name);
  }

  /* Shared Memory Buffer Initialization */
  key_t key;
  int id_memory;
  message *memory;

  key = ftok(key_route, 33);
  if (key == -1) {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (getBuffer(&id_memory, key, buffer_size) == 0) {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (getBuffer(&id_memory, key, buffer_size) == 0) {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (getBufferMemory(&id_memory, &memory) == 0) {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Get Semaphore */
  int id_semaphore;
  if (checkBinDir()) {
    id_semaphore = initSemaphore("../share_files/sem", buffer_size);
  } else {
    id_semaphore = initSemaphore("share_files/sem", buffer_size);
  }

  /* Up to all the semaphores */

  struct sembuf operation;
  operation.sem_flg = 0;

  operation.sem_num = buffer_size;
  operation.sem_op = -1;
  semop(id_semaphore, &operation, 1);

  memory2->kill = 1;

  operation.sem_op = 1;
  semop(id_semaphore, &operation, 1);

  operation.sem_num = buffer_size + 1;
  operation.sem_op = buffer_size;
  semop(id_semaphore, &operation, 1);

  operation.sem_num = buffer_size + 2;
  operation.sem_op = buffer_size;
  semop(id_semaphore, &operation, 1);

  printColor("Stopping all process....\n", 2);

  while ((memory2->producers > 0) || (memory2->consumers > 0)) {
    sleep(0.5);
  }

  printColor("All process stopped\n", 2);

  /* Retreive the global memory variables */
  int unread_msg = getUnreadMessages(buffer_size, memory);

  double kernel = memory2->blocked_time + memory2->waiting_time;

  printFinalizerEnd(memory2->messages, unread_msg, memory2->total_consumers,
                      memory2->total_producers, memory2->magic,
                      memory2->waiting_time, memory2->blocked_time,
                      memory2->user_time, kernel);

  /* Clears semaphores and shared memory for buffer and global variables */

  shmctl(id_memory, IPC_RMID, (struct shmid_ds *)NULL);

  shmctl(gv_shm_id, IPC_RMID, (struct shmid_ds *)NULL);

  semctl(id_semaphore, 0, IPC_RMID, NULL);

  /* Deletes files for shared memory and semaphore */

  if (checkBinDir()) {
    system("rm -rf ../share_files");
  } else {
    system("rm -rf share_files");
  }

  return 0;
}
