#include "../include/shmem.h"

/* Creates the buffer for shared memory */
int createBuffer(int *id, key_t mem_key, int mem_size) {
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777 | IPC_CREAT);
  if (*id == -1) {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer id for shared memory */
int getBuffer(int *id, key_t mem_key, int mem_size) {
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777);
  if (*id == -1) {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer memory for shared memory */
int getBufferMemory(int *id, message **memory) {
  *memory = (message *)shmat(*id, (char *)0, 0);
  if (*memory == NULL) {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}

/* Creates the global variables for shared memory */
int createGlobal(int *id, key_t mem_key) {
  *id = shmget(mem_key, 1 * sizeof(globalVars), 0777 | IPC_CREAT);
  if (*id == -1) {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables id for shared memory */
int getGlobal(int *id, key_t mem_key) {
  *id = shmget(mem_key, 1 * sizeof(globalVars), 0777);
  if (*id == -1) {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables memory for shared memory */
int getGlobalMemory(int *id, globalVars **memory) {
  *memory = (globalVars *)shmat(*id, (char *)0, 0);
  if (*memory == NULL) {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}

/* Return is used */
int getIndex(int flag, int size, message *memory, int ref, int id_semaphore) {
  struct sembuf operation;
  while (1) {
    operation.sem_num = ref % size;
    operation.sem_op = -1;
    semop(id_semaphore, &operation, 1);
    if (memory[ref % size].is_used == flag) {
      operation.sem_op = 1;
      semop(id_semaphore, &operation, 1);
      return ref % size;
    }
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);
    ref++;
  }
}

/*Set is_used*/
void initMemory(int size, message *memory) {
  for (int i = 0; i < size; i++) {
    memory[i].is_used = 0;
    memory[i].pid = 0;
    memory[i].magic_number = 0;
    memset(memory[i].date, 0, 50 * sizeof(char));
  }
}

int getUnreadMessages(int size, message *memory) {
  int counter = 0;
  for (int i = 0; i < size; ++i) {
    if (memory[i].is_used) {
      counter++;
    }
  }

  return counter;
}
