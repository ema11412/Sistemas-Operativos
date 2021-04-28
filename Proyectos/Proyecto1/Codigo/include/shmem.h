#ifndef _SHMEM_H
#define _SHMEM_H

#include <stdio.h>
#include <string.h>
#include <structs.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>

/* Library to create and manage shared memory */

int createBuffer(int *id, key_t mem_key, int mem_size);
int createGlobal(int *id, key_t mem_key);
int getBuffer(int *id, key_t mem_key, int mem_size);
int getGlobal(int *id, key_t mem_key);
int getBufferMemory(int *id, message **memory);
int getGlobalMemory(int *id, globalVars **memory);
int getIndex(int flag, int size, message *memory, int ref, int id_semaphore);
void initMemory(int size, message *memory);
int getUnreadMessages(int size, message *memory);

#endif
