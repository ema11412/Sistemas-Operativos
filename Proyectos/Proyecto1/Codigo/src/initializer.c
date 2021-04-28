#include "../include/semaphore.h"
#include "../include/shmem.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/structs.h"
#include <sys/sem.h>
#include "../include/utilities.h"

int main(int argc, char *argv[]) {
  key_t key;
  int id_memory;
  message *memory = NULL;

  key_t key_global;
  int gv_shm_id;
  globalVars *memory2 = NULL;

  /* Argument Validation */
  if (argc != 3) {
    printf("Usage: ./initializer <buffer_size> <buffer_name>\n");
    exit(0);
  }

  int buffer_size = atoi(argv[1]);
  char *buffer_name = argv[2];

  if (buffer_size == 0 || !isNumber(argv[1])) {
    printf("Please insert a correct buffer size\n");
    exit(0);
  }

  /* Check and init directories */
  createDirs(buffer_name);

  /* Shared Memory Buffer Initialization */
  char *key_route;
  if (checkBinDir()) {
    key_route = concat("../share_files/", buffer_name);
  } else {
    key_route = concat("share_files/", buffer_name);
  }

  checkDir(key_route);

  key = ftok(key_route, 33);
  if (key == -1) {
    printf("Buffer Key is Invalid\n");
    exit(0);
  }

  if (createBuffer(&id_memory, key, buffer_size) == 0) {
    printf("Can't create buffer memory\n");
    exit(0);
  }

  if (getBufferMemory(&id_memory, &memory) == 0) {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Shared Memory for Global Variables Initialization */
  if (checkBinDir()) {
    key_global = ftok("../share_files/global", 33);
  } else {
    key_global = ftok("share_files/global", 33);
  }

  if (key_global == -1) {
    printf("Global Variables Key is Invalid\n");
    exit(0);
  }

  if (createGlobal(&gv_shm_id, key_global) == 0) {
    printf("Can't create global memory\n");
    exit(0);
  }

  if (getGlobalMemory(&gv_shm_id, &memory2) == 0) {
    printf("Can't get global memory\n");
    exit(0);
  }

  memory2->size = buffer_size;

  /* Create Semaphore */
  if (checkBinDir()) {
    createSemaphore("../share_files/sem", buffer_size);
  } else {
    createSemaphore("share_files/sem", buffer_size);
  }

  /* Set buffer to zero */
  initMemory(buffer_size, memory);

  return 0;
}