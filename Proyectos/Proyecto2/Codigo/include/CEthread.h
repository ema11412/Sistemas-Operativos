#ifndef CETHREAD
#define CETHREAD

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h> /* For clone */
#include <signal.h> /* For SIGCHLD */
#include <sys/types.h> /* For pid_t */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* For getpid */
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h> // For call to gettid

/* The maximum number of fibers that can be active at once. */
#define MAX_FIBERS 300
/* The size of the stack for each fiber. */
#define FIBER_STACK (1024*1024)

#define    CE_NOERROR    0
#define    CE_MAXFIBERS    1
#define CE_MALLOCERROR    2
#define CE_CLONEERROR    3
#define    CE_INFIBER    4
#define CE_SIGNALERROR    5

/* The CEthread Structure
*  Contains the information about individual CEthreads.
*/
typedef struct {
    pid_t pid; /* The pid of the child thread as returned by clone */
    void *stack; /* The stack pointer */
    char detached; // Detached or not
    int killed;
} CEthread_t;

typedef struct {
    int id;
} CEthread_attr_t;

typedef struct {
    char locked;
    pid_t pid;
} CEthread_mutex_t;
typedef struct {
    int id;
} CEthread_mutexattr_t;

/*
@param: CEthread_create: Creates a new thread
CEthread_t* thread: Pointer to thread structure
const CEthread_attr_t* attr: pointer to thread attribute structure
const CEthread_attr_t* attr: pointer to thread attribute structure
int (*start_routine)(void*): Pointer to routine to start
void* arg: Argument for the routine
*/

int CEthread_create(CEthread_t *thread, const CEthread_attr_t *attr, int (*start_routine)(void *), void *arg);

/*
CEthread_exit: Kills a thread
CEthread_t thread: Thread structure
*/

int CEthread_exit(int pid);

/*
CEthread_yield: Yields a thread
*/

int CEthread_yield();

/*
CEthread_join: Joins a thread and waits for it to finish
CEthread_t thread: Thread structure
void** retval: return value of the function
*/

int CEthread_join(int pid, void **retval);

/*
CEthread_detachs: Detachs a thread and makes it unjoinable
CEthread_t thread: Thread structure
*/

int CEthread_detach(CEthread_t thread);

/*
CEmutex_init: Initializes a mutex strcuture
CEthread_mutex_t* restrict mutex: Pointer to the structure
const CEthread_mutexattr_t *restrict attr: Pointer to attribute 
*/

int CEmutex_init(CEthread_mutex_t *restrict mutex, const CEthread_mutexattr_t *restrict attr);

/*
CEmutex_destroy: Destroys a mutex strcuture
CEthread_mutex_t* mutex: Pointer to the structure
*/

int CEmutex_destroy(CEthread_mutex_t *mutex);

/*
CEmutex_unlock: Unlock a mutex strcuture
CEthread_mutex_t* mutex: Pointer to the structure
*/

int CEmutex_unlock(CEthread_mutex_t *mutex);

/*
CEmutex_trylock: Tries to lock a mutex strcuture
CEthread_mutex_t* mutex: Pointer to the structure
*/

int CEmutex_trylock(CEthread_mutex_t *mutex);

/*
CEmutex_lock: Locks a mutex strcuture
CEthread_mutex_t* mutex: Pointer to the structure
*/

int CEmutex_lock(CEthread_mutex_t *mutex);

/*
CEmutex_end: Ends a thread called from itself
*/

void CEthread_end();

/*
init_threads: Starts the threads system
*/

void init_threads();

/*
map_pid_index: Maps a pid to an index in the list
pid_t id: pid to search
*/

int map_pid_index(pid_t id);

/*
waitForAllCEthreads: Waits for all the pthreads to end
*/

int waitForAllCEthreads();

/*
sync_printf: Printf blocked with CEthread mutex
char* format: Format for the string
*/

void sync_printf(char *format, ...);

/*
convert: Converts number to a base 
unsigned int num: number input
int base: base input
*/


char *convert(unsigned int num, int base);

#endif