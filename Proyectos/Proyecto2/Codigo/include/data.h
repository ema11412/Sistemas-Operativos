#ifndef _DATA_H
#define _DATA_H

#include "linkedlist.h"

typedef struct {
  int type;
  char direction;
  int usedUp;
  int usedDown;
} cell;


typedef struct{
  char *name;
  int capacity;
  int length;
  int counter;
  short lastAccess;
  float accumulator;
  int algorithm;
  int scheduler;
  int w_value ;
  double tic;
  float m_time;
  float n_time;
}channel;

typedef struct{
  int type;
  int isAlive;
  int posi;
  int posj;
  int lmove;
  int move;
  int pid;
  int weight;
  int burstTime;
  int priority;
  double tic;
  double velocity;
  char direction;
}ant;

typedef struct{
  int probTypeA;
  int probTypeB;
  int probTypeC;
  int mean;
  int quantum;
} configs;


typedef struct{
 llist *Up;
 llist *Down;
 channel *Channel;
} dataScheduler;


int getChannelData(channel *channel, char* filename);
int moveAnt(ant *Ant, cell map[24][46]);
ant * createAnt(int baseVel, int home, int Types[100]);
ant * createAntManual(int baseVel, int home, int Type);
void print(ant Ant);
int getConfigsData(configs *conf, char* filename);
void loadMap(cell map[24][46]);
int getAntPath(ant *Ant, cell map[24][46], char dir);
int Equidad(llist* up,llist* down,channel* p,int W);
int Letrero_Algorithm(channel* p,float timeN,float timeS);
int Tico_Algorithm(llist* up,llist* down,channel* p);
int FIFO(llist *Up, llist *Down, channel *channel,cell map[24][46]);
int Priority(llist *Up, llist *Down, channel *Channel,cell map[24][46]);
llist* sortQueuePriority(llist* readyQueue);
int RR_Scheduler(llist *Up, llist *Down, channel *channel,cell map[24][46], int quantum);
llist* RR_SortingAlgorithm(llist* readyQueue, int quantum, int NOP);
int SJF_Scheduler(llist *Up, llist *Down, channel *channel,cell map[24][46]);
dataScheduler *createDataScheduler(llist * Up, llist * Down,channel *Channel);

#endif 