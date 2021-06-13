#define _GNU_SOURCE

#include "../../include/CEthread.h"

// List of threads
static CEthread_t CEthreadList[MAX_FIBERS];
// Required pids
static pid_t parent_pid;
static pid_t group_pid;
// Counter for threads
static int numCEthreads = 0;
// Variable to check if it already started
static char started_ = 0;

int CEthread_create(CEthread_t *thread, const CEthread_attr_t *attr, int (*start_routine)(void *), void *arg) {

    
    if (started_ == 0) { // Singleton for the system
        printf("HOla soy emanuel");
        init_threads();
        started_ = 1;
    }

    /*Allocate the stack*/
    thread->stack = malloc(FIBER_STACK);
    if (thread->stack == 0) {
        printf("Error: Could not allocate stack.\n");
        return CE_MALLOCERROR;
    }

    /* Call the clone system call to create the child thread */
    thread->pid = clone(start_routine, (char *) (thread->stack + FIBER_STACK),
                        SIGCHLD |
                        CLONE_FS |
                        CLONE_FILES |
                        CLONE_SIGHAND |
                        CLONE_VM |
                        CLONE_CHILD_CLEARTID |
                        CLONE_PTRACE, arg);
    if (thread->pid == -1) { // Error in clone
        free(thread->stack);
        printf("Error: clone system call failed.\n");
        return CE_CLONEERROR;
    }

    // E  E  E  E  E  E  E  E
    // Copies thread to list
    thread->detached = 0;
    thread->killed = 0;
    if (numCEthreads == 0) {
        memcpy((void *) &CEthreadList[numCEthreads], (void *) thread, sizeof(CEthread_t));
        return CE_NOERROR;
    } else {
        for (int i = 0; i <= numCEthreads; i++) {
            if (CEthreadList[i].killed == 1) {
                memcpy((void *) &CEthreadList[i], (void *) thread, sizeof(CEthread_t));
                return CE_NOERROR;
            }
        }
        if (numCEthreads < MAX_FIBERS - 1) {
            memcpy((void *) &CEthreadList[numCEthreads++], (void *) thread, sizeof(CEthread_t));
            return CE_NOERROR;
        }
    }
    return CE_MAXFIBERS;
}


int CEthread_exit(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        printf("thread(killed), id = %d\n", pid);
        int index = map_pid_index(pid);
        CEthreadList[index].killed = 1;
        return 0;
    } else {
        return 1;
    }
}

int CEthread_yield() {
    /* Call the sched_yield system call which moves the current process to the
    end of the process queue. */
    sched_yield();
    return 0;
}

int CEthread_join(int pid, void **retval) {
    int index = map_pid_index(pid);

    if (CEthreadList[index].detached == 0) {
        waitpid(pid, 0, 0); // Key is here, wait for it to end
        printf("%s\n", "done join");
        CEthreadList[index].killed = 1;

        return 0;
    } else {
        return 1;
    }
}

int CEthread_detach(CEthread_t thread) {
    int index = map_pid_index(thread.pid);
    CEthreadList[index].detached = 1;
    return 0;
}

int CEmutex_init(CEthread_mutex_t *restrict mutex, const CEthread_mutexattr_t *restrict attr) {
    mutex->locked = 0; // Set the mutex as unlocked
    mutex->pid = 0;
    return 0;
}

int CEmutex_destroy(CEthread_mutex_t *mutex) {
    mutex->locked = 0; // Set the mutex as unlocked
    mutex->pid = 0;
    return 0;
}

int CEmutex_unlock(CEthread_mutex_t *mutex) {
    mutex->locked = 0; // Set the mutex as unlocked
    mutex->pid = 0;
    return 0;
}

int CEmutex_trylock(CEthread_mutex_t *mutex) {
    if (mutex->locked == 0) { // If mutex is not locked, lock it
        mutex->locked = 1;
        mutex->pid = getpid();
        return 0;
    }
    return 1;
}

int CEmutex_lock(CEthread_mutex_t *mutex) {
    LOOP:
    while (mutex->locked); // Race condition !!!!!!!!! Wait for mutex to unlock
    pid_t id = getpid();
    mutex->locked = 1;
    mutex->pid = id;
    if (mutex->pid != id) { // Method one to eliminate race condition
        goto LOOP;
    }
    return 0;
}

void CEthread_end() {
    // Kills the thread
    killpg(getpgrp(), SIGKILL);
}

void init_threads() {
    // Initialices
    for (int i = 0; i < MAX_FIBERS; ++i) {
        CEthreadList[i].pid = 0;
        CEthreadList[i].stack = 0;
    }
    // Signal from terminal
    signal(SIGINT, CEthread_end);
    atexit(CEthread_end); // When parent ends
    // Sets the required pids
    group_pid = getpgrp();
    parent_pid = getpid();
}

int map_pid_index(pid_t id) {
    // Search for that pid
    for (int i = 0; i < MAX_FIBERS; ++i) {
        if (CEthreadList[i].pid == id) {
            return i;
        }
    }
    return -1;
}

int wait_all() {
    printf("%s\n", "Calling wait");
    pid_t pid;
    int i;
    int CEthreadsRemaining = 0;

    /* Check to see if we are in a CEthread, since we don't get signals in the child threads */
    pid = getpid();
    if (pid != parent_pid) return CE_INFIBER;

    /* Wait for the CEthreads to quit, then free the stacks */
    while (numCEthreads > CEthreadsRemaining) {
        pid = wait(0); //key here
        if (pid == -1) {
            printf("Error: wait system call failed.\n");
            exit(1);
        }

        /* Find the CEthread, free the stack, and swap it with the last one */
        for (i = 0; i < numCEthreads; ++i) {
            if (CEthreadList[i].pid == pid) {
                printf("Child CEthread pid = %d exited\n", pid);
                numCEthreads--;

                free(CEthreadList[i].stack);
                if (i != numCEthreads) {
                    CEthreadList[i] = CEthreadList[numCEthreads];
                }

                i = -1;
                break;
            }
        }
        if (i == numCEthreads) {
            printf("Did not find child pid = %d in the CEthread list\n", pid);
        }
    }

    return CE_NOERROR;
}

void sync_printf(char *format, ...) {
    static CEthread_mutex_t lock = {0, 0};
    CEmutex_lock(&lock);
    printf("%s\n", "Locked");
    va_list arg;
    va_start(arg, format);
    vprintf(format, arg);
    va_end(arg);
    CEmutex_unlock(&lock);
    printf("%s\n", "Unlocked");
}

char *convert(unsigned int num, int base) {
    static char Representation[] = "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = Representation[num % base];
        num /= base;
    } while (num != 0);

    return (ptr);
}