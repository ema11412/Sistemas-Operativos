#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

/* Library to create and init semaphores */

int initSemaphore(char *buff_name, int buffer_size);
void createSemaphore(char *route, int buffer_size);
void upSemaphore(int id, int index);
void downSemaphore(int id, int index);

#endif
