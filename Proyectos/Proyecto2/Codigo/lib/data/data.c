#include <libconfig.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "../../include/data.h"

/**
 * Fills a channel struct froma a config file
 * @param BR: Struct to fill
 * @param filename: Path of the cofig file
 * @return 0 if all is ok 1 if have some trouble
*/
int getChannelData(channel *BR, char* filename){
  config_t cfg, *cf;
  const char *name = NULL;
  int capacity, length, algorithm, scheduler, W;
  double m_time,n_time;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, filename)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return 1;
	}

  if(!config_lookup_string(cf,"Name", &name)){
    printf("Error en Name\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Capacity", &capacity)){
    printf("Error en Capacity\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Length", &length)){
    printf("Error en Length\n");
    return 1;
  }

  if(!config_lookup_int(cf,"Algorithm", &algorithm)){
    printf("Error en Algorithm\n");
    return 1;
  }
  
  if(!config_lookup_int(cf,"Scheduler", &scheduler)){
    printf("Error en Scheduler\n");
  }


  if(!config_lookup_int(cf,"W_value", &W)){
    printf("Error en W_value\n");
    return 1;
  }
  if(!config_lookup_float(cf,"Letrero_Time_M", &m_time)){
    printf("Error en Letrero_Time_M\n");
    return 1;
  }
  if(!config_lookup_float(cf,"Letrero_Time_N", &n_time)){
    printf("Error en Letrero_Time_N\n");
    return 1;
  }

  BR->name = (char*)name;
  BR->algorithm = algorithm;
  BR->scheduler = scheduler;
  
  BR->w_value = W;
  BR->capacity = capacity;
  BR->length = length;
  BR->counter = 0;
  BR->lastAccess = 1;
  BR->accumulator = 0;
  BR->m_time = m_time;
  BR->n_time = n_time;  
  return 0;
}

/**
 * Move an Ant to a next position
 * 
*/
int moveAnt(ant *Ant, cell map[24][46]){
  int i = Ant->posi;
  int j = Ant->posj;
  
  int p[4]= {-1, 0, 1, 0};
  int q[4]= {0, 1, 0, -1};

  int iter = 0;

  if (Ant->lmove == 1){
    iter = 0;
  } else if (Ant->lmove == 2){
    iter = 1;
  } else if (Ant->lmove == 3){
    iter = 2;
  } else if (Ant->lmove == 4){
    iter = 3;
  }

  //(i == 5 && j == 21)

  for(int x = 0; x < 4; x++){
    char mapDir = map[i + p[iter]][j + q[iter]].direction;
    int nextUp = map[i + p[iter]][j + q[iter]].usedUp;
    int nextDown = map[i + p[iter]][j + q[iter]].usedDown;
    if(mapDir == 'C'){
      if(Ant -> direction == 'B' && nextDown){
        break;
      } else if(Ant -> direction == 'A' && nextUp){
        break;
      }
      Ant->posi = i + p[iter];
      Ant->posj = j + q[iter];
      int cmp = getAntPath(Ant,map,Ant->direction);
      if(cmp != 5){
        Ant->lmove = cmp;
      }else if(Ant->posi == 5 && Ant -> posj == 11 && Ant->direction == 'B'){
        Ant->lmove = 3;
      }else if(Ant->posi == 17 && Ant -> posj == 11 && Ant->direction == 'B'){
        Ant->lmove = 2;
      }else if(Ant->posi == 17 && Ant -> posj == 33 && Ant->direction == 'A'){
        Ant->lmove = 1;
      }else if(Ant->posi == 5 && Ant -> posj == 33 && Ant->direction == 'A'){
        Ant->lmove = 4;
      }else{
        Ant->lmove = iter+1;
      }
      if(Ant->direction == 'B'){
        map[i][j].usedDown = 0;
        map[Ant->posi][Ant->posj].usedDown = 1;
      }else{
        map[i][j].usedUp = 0;
        map[Ant->posi][Ant->posj].usedUp = 1;
      }
      return 0;
    } else if(Ant -> direction == mapDir){
      if(Ant -> direction == 'B' && nextDown){
        break;
      } else if(Ant -> direction == 'A' && nextUp){
        break;
      }
      Ant->posi = i + p[iter];
      Ant->posj = j + q[iter];
      if(Ant->posi == 21 && Ant -> posj == 20){
        Ant->lmove = 2;
      }else if(Ant->posi == 21 && Ant -> posj == 43){
        Ant->lmove = 1;
      }else if(Ant->posi == 1 && Ant -> posj == 24){
        Ant->lmove = 4;
      }else if(Ant->posi == 1 && Ant -> posj == 1){
        Ant->lmove = 3;
      }else{
        Ant->lmove = iter + 1;
      }
      if(Ant->direction == 'B'){
        map[i][j].usedDown = 0;
        map[Ant->posi][Ant->posj].usedDown = 1;
      }else{
        map[i][j].usedUp = 0;
        map[Ant->posi][Ant->posj].usedUp = 1;
      }
      return 0;
    } 
    iter ++;
    if (iter == 4)
      iter = 0;
  }
  return 1;
}

int getAntPath(ant *Ant, cell map[24][46], char dir){
  int i = Ant -> posi;
  int j = Ant -> posj;
  if(map[i-1][j].direction == dir){
    return 1;
  } else if(map[i][j+1].direction == dir){
    return 2;
  } else if(map[i+1][j].direction == dir){
    return 3;
  } else if(map[i][j-1].direction == dir){
    return 4;
  } else{
    return 5;
  }
}

/**
 * Create a new Ant
 * @param baseVel: Base Velocity of the ants
 * @param home: Community of the ant
 * @return New ant struct
*/

ant * createAnt(int baseVel, int home, int Types[100]){
  ant *newAnt = malloc(sizeof(ant));
  int type = rand()%100;
  int mul = (rand() % (200 - 50 + 1)) + 50;
  int weight = (rand() % (40 - 20 + 1)) + 20;

  struct timeval tic;
  gettimeofday(&tic, NULL);
  newAnt->tic = (double)tic.tv_sec;
  
  newAnt -> weight = weight;
  newAnt -> type =  Types[type];
  newAnt -> isAlive = 1;
  newAnt -> lmove = 0;
  newAnt -> move = 1;
  if (newAnt->type == 0){
    newAnt -> velocity = baseVel;
  } else if (newAnt->type == 1){
    newAnt -> velocity = baseVel + baseVel * 0.2;
  } else if (newAnt->type == 2){
    newAnt -> velocity = baseVel + baseVel * mul / 100 ;
  }
  if(home){
    newAnt -> direction = 'B';
    //newAnt -> posi = 3;
    //newAnt -> posj = 22;
    newAnt -> posi = 1;
    newAnt -> posj = 22;
  }else{
    newAnt -> direction = 'A';
    newAnt -> posi = 22;
    newAnt -> posj = 22;
    //newAnt -> posi = 23;
    //newAnt -> posj = 20;
  }

  return newAnt;
}

/**
 * Create a new Ant
 * @param baseVel: Base Velocity of the ants
 * @param home: Community of the ant
 * @return New ant struct
*/

ant * createAntManual(int baseVel, int home, int Type){
  ant *newAnt = malloc(sizeof(ant));
  int mul = (rand() % (200 - 50 + 1)) + 50;
  int weight = (rand() % (40 - 20 + 1)) + 20;
  struct timeval tic;
  gettimeofday(&tic, NULL);

  newAnt -> weight = weight;
  newAnt->tic = (double)tic.tv_sec;
  newAnt -> type =  Type;
  newAnt -> isAlive = 1;
  newAnt -> lmove = 0;
  newAnt -> move = 1;
  if (newAnt->type == 0){
    newAnt -> velocity = baseVel;
  } else if (newAnt->type == 1){
    newAnt -> velocity = baseVel + baseVel * 0.2;
  } else if (newAnt->type == 2){
    newAnt -> velocity = baseVel + baseVel * mul / 100 ;
  }
  if(home){
    newAnt -> direction = 'B';
    newAnt -> posi = 1;
    newAnt -> posj = 22;
  }else{
    newAnt -> direction = 'A';
    newAnt -> posi = 22;
    newAnt -> posj = 22;
  }

  return newAnt;
}

/**
 * Prints ant values
 * @param Ant: Ant to print
*/

void print(ant Ant){
  printf("Tipo: %d\n", Ant.type);
  printf("IsAlive: %d\n", Ant.isAlive);
  printf("Velocidad: %.3f\n", Ant.velocity);
  printf("Direccion: %c\n", Ant.direction);
}

/**
 * Fills a channel struct froma a config file
 * @param BR: Struct to fill
 * @param filename: Path of the cofig file
 * @return 0 if all is ok 1 if have some trouble
*/
int getConfigsData(configs *conf, char* filename){
  config_t cfg, *cf;
  
  int probTypeA, probTypeB, probTypeC, mean, quantum;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, filename)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return 1;
	}

  if(!config_lookup_int(cf,"probTypeA", &probTypeA)){
    printf("Error en probTypeA\n");
    return 1;
  }

  if(!config_lookup_int(cf,"probTypeB", &probTypeB)){
    printf("Error en probTypeB\n");
    return 1;
  }

  if(!config_lookup_int(cf,"probTypeC", &probTypeC)){
    printf("Error en probTypeC\n");
    return 1;
  }

  if(!config_lookup_int(cf,"mean", &mean)){
    printf("Error en Mean\n");
    return 1;
  }

    if(!config_lookup_int(cf,"Quantum", &quantum)){
    printf("Error en Quantum\n");
    return 1;
  }
  
  conf -> probTypeA = probTypeA;
  conf -> probTypeB = probTypeB;
  conf -> probTypeC = probTypeC;
  conf -> mean = mean;
  conf -> quantum = quantum;

  return 0;
}

void loadMap(cell map[24][46]){
  FILE * file;
  file = fopen("../assets/map/map2.map", "r");
  char ch;
  int i = 0;
  int j = 0;
  while ((ch = fgetc(file)) != EOF){
   if(ch == '\n'){
     i += 1;
     j = 0;
   } else if (ch == '-'){
     map[i][j].type = 0;
     map[i][j].direction = 'D';
     j += 1;
   } else if (ch == 'x'){
     map[i][j].type = 1;
     map[i][j].direction = 'A';
     j += 1;
   } else if (ch == 'o'){
     map[i][j].type = 2;
     map[i][j].direction = 'C';
     j += 1;
   } else if (ch == 'p'){
     map[i][j].type = 3;
     map[i][j].direction = 'C';
     j += 1;
   } else if (ch == 'z'){
     map[i][j].type = 1;
     map[i][j].direction = 'B';
     j += 1;
   }
  }

  fclose(file);
}

int Equidad(llist* up,llist* down,channel* p,int W) {
// 1 = los que vienen de arriba, 0 = los que vienen de abajo
  int lenUp = llist_getSize(up);
  int lenDown = llist_getSize(down);
  int direction = p->lastAccess;
  if(lenUp > lenDown){ //Si hay mas ants arriba 
    if (direction){ //Si direccion es 1
      if(p->counter < W){ //Si elcontador es menor a Y
        p->counter+=1; //Deje pasar 1 ant
        //printf("Dejan pasar a un ant(arriba), contador=%d\n",p->counter);
        if (p->counter == W){ //El contador es igual a Y?
          p->counter = 0; // reinicia el contador
          p->lastAccess = 0; //cambia la direccion a 0
        }        
        return 1; // Si el contador aun es menor a Y, sigue la misma direccion
      }
    }else{
      if (!direction){ //Si direccion es 0
        if(p->counter < W){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 ant
          //printf("Dejan pasar a un ant(abajo), contador=%d\n",p->counter);
          if (p->counter == W){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
          }        
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
  } else if(lenUp < lenDown){
    if (!direction) {
      if(p->counter < W){
        p->counter+=1;
        //printf("Dejan pasar a un ant(abajo), contador=%d\n",p->counter);
        if (p->counter == W){
          p->counter = 0;
          p->lastAccess = 1;
        }
        return 0;
      }
    }else{
      if (direction){ //Si direccion es 1
        if(p->counter < W){
          p->counter+=1; //Deje pasar 1 ant
          //printf("Dejan pasar a un ant(arriba), contador=%d\n",p->counter);
          if (p->counter == W){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }        
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
  }else{ 
      if (direction){ //Si direccion es 1
        if(p->counter < W && lenUp != 0){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 ant
          //printf("Dejan pasar a un ant(arriba), contador=%d\n",p->counter);
          if (p->counter == W){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 0; //cambia la direccion a 0
          }
          return 1; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }else{
        if(p->counter < W && lenDown != 0){ //Si elcontador es menor a Y
          p->counter+=1; //Deje pasar 1 ant
          //printf("Dejan pasar a un ant(abajo), contador=%d\n",p->counter);
          if (p->counter == W){ //El contador es igual a Y?
            p->counter = 0; // reinicia el contador
            p->lastAccess = 1; //cambia la direccion a 0
          }
          return 0; // Si el contador aun es menor a Y, sigue la misma direccion
        }
      }
    }
return direction;
}

int Letrero_Algorithm(channel* p,float timeN,float timeS){
  struct timeval tic;
  float accum = p->accumulator;

  gettimeofday(&tic, NULL); //toma el tiempo
  p->tic = (double)tic.tv_sec;
  
  if(p->lastAccess && accum >= timeN){  
    p->lastAccess = 0;
    p->accumulator = 0;          
  }
  else if(!(p->lastAccess) && accum >= timeS){   
    p->lastAccess = 1;
    p->accumulator = 0;        
  } 
  return p->lastAccess;
}

int Tico_Algorithm(llist* up,llist* down,channel* p){ 
  int temp;
  temp = Equidad(up,down,p,1);
  return temp;
}


int FIFO(llist *Up, llist *Down, channel *Channel,cell map[24][46]){
  int getAntFrom = 0;
  ant *Ant;
  float accum = Channel->accumulator;
  struct timeval toc;

  gettimeofday(&toc, NULL);
  double time_Dif = (double)(toc.tv_sec - Channel->tic);
  accum += time_Dif;
  Channel->accumulator = accum;

  //Chose the algorithm
  if(Channel->algorithm == 0){
    getAntFrom = Equidad(Up,Down,Channel,Channel->w_value);
  }
  if(Channel->algorithm == 1){
    getAntFrom = Letrero_Algorithm(Channel,Channel->m_time, Channel->n_time);
  }  
  if(Channel->algorithm == 2){
    getAntFrom = Tico_Algorithm(Up,Down,Channel);
  }

  //Get Ant form Qeues
  if(getAntFrom){
    if(llist_getSize(Up) == 0){
      return 0;
    }
    Ant = (ant *)llist_getbyId(Up,0);
    if(Ant->posi == 9 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedDown = 0;
      Ant -> posi += 2;
    }
  } else{
    if(llist_getSize(Down) == 0){
      return 0;
    }
    Ant = (ant *)llist_getbyId(Down,0);
    if(Ant->posi == 13 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedUp = 0;
      Ant -> posi -= 2;
    }
  }


  if(Ant->move){
    return 0;
  }

  double wait = (double)Channel->length / (double)Ant->velocity;
  usleep(wait * 1000000);

  if(getAntFrom){
    Ant -> posi += 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Up,0);
    }
  } else{
    Ant -> posi -= 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Down,0);
    }
  }
  return 0;
}

int Priority(llist *Up, llist *Down, channel *Channel,cell map[24][46]){
  int getAntFrom = 0;
  ant *Ant;
  float accum = Channel->accumulator;
  struct timeval toc;
  Up = sortQueuePriority(Up);
  Down = sortQueuePriority(Down);
  
  gettimeofday(&toc, NULL);
  double time_Dif = (double)(toc.tv_sec - Channel->tic);
  accum += time_Dif;
  Channel->accumulator = accum;

  //Chose the algorithm
  if(Channel->algorithm == 0){
    getAntFrom = Equidad(Up,Down,Channel,Channel->w_value );
  }
  if(Channel->algorithm == 1){
    getAntFrom = Letrero_Algorithm(Channel,Channel->m_time, Channel->n_time);
  }  
  if(Channel->algorithm == 2){
    getAntFrom = Tico_Algorithm(Up,Down,Channel);
  }

  //Sorting the queue
  if(getAntFrom){
    if(llist_getSize(Up) == 0){
      return 0;
    }
    Ant = (ant *)llist_getbyId(Up,0);
    if((Ant->posi <= 9 && Ant->posi >= 5 ) && (Ant->posj == 11 || Ant->posj == 22 || Ant->posj == 33 )){
      map[Ant->posi][Ant->posj].usedDown = 0;
      Ant->move = 0;
      Ant -> posi = 11;
      llist_delById(Up,0);
    }
  } else{
    if(llist_getSize(Down) == 0){
      return 0;
    }
    Ant = (ant *)llist_getbyId(Down,0);
    if((Ant->posi <= 17 && Ant->posi >= 13 ) && (Ant->posj == 11 || Ant->posj == 22 || Ant->posj == 33 )){
      map[Ant->posi][Ant->posj].usedUp = 0;
      Ant->move = 0;
      Ant -> posi = 11;
      llist_delById(Down,0);
    }
  }

  if(Ant->move){
    return 0;
  }

  double wait = (double)Channel->length / (double)Ant->velocity;
  usleep(wait * 1000000);

  if(getAntFrom){
    Ant -> posi += 2;
    Ant -> move = 1;
  } else{
    Ant -> posi -= 2;
    Ant -> move = 1;
  }
  return 0;
}

llist* sortQueuePriority(llist* readyQueue){

  ant* Ant;
  llist* sortingList = NULL;

  for (int i = 0; i < llist_getSize(readyQueue); i ++){

    Ant = (ant*)llist_getbyId(readyQueue,i);
    Ant->priority = i + 1;
    Priority_SortingAlgorithm(sortingList);

  }

  return readyQueue;

}


int RR_Scheduler(llist *Up, llist *Down, channel *Channel,cell map[24][46], int quantum){

  int getAntFrom = 0;
  int NOP = 0; // Number of processes
  llist* UpSort;
  llist* DownSort;
  ant *Ant;
  float accum = Channel->accumulator;
  struct timeval toc;

  gettimeofday(&toc, NULL);
  double time_Dif = (double)(toc.tv_sec - Channel->tic);
  accum += time_Dif;
  Channel->accumulator = accum;

  if(Channel->algorithm == 0){
    getAntFrom = Equidad(Up,Down,Channel,Channel->w_value );
  }
  if(Channel->algorithm == 1){
    getAntFrom = Letrero_Algorithm(Channel,Channel->m_time, Channel->n_time);
  }  
  if(Channel->algorithm == 2){
    getAntFrom = Tico_Algorithm(Up,Down,Channel);
  }

  //Sorting the queue
  if(getAntFrom){
    if(llist_getSize(Up) == 0){
      return 0;
    }
    NOP = llist_getSize(Up);
    UpSort = RR_SortingAlgorithm(Up,quantum, NOP);
    Ant = (ant *)llist_getbyId(UpSort,0);
    if(Ant->posi == 9 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedDown = 0;
      Ant -> posi += 2;
    }
  } else{
    if(llist_getSize(Down) == 0){
      return 0;
    }
    NOP = llist_getSize(Down);
    DownSort = RR_SortingAlgorithm(Down,quantum, NOP);
    Ant = (ant *)llist_getbyId(DownSort,0);
    if(Ant->posi == 13 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedUp = 0;
      Ant -> posi -= 2;
    }
  }


  if(Ant->move){
    return 0;
  }

  double wait = (double)Channel->length / (double)Ant->velocity;
  usleep(wait * 1000000);

  if(getAntFrom){
    Ant -> posi += 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Up,0);
    }
  } else{
    Ant -> posi -= 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Down,0);
    }
  }
  return 0;

}


llist* RR_SortingAlgorithm(llist* readyQueue, int quantum, int NOP){

  ant *Ant;
  int i = 0, sum = 0, count = 0, waitingTime = 0, turnAroundTime = 0, y = NOP;
  int arrivalCounter = 0;
  int burstTime[256], temp[256], arrivalTime[256];

  for(int i = 0; i < NOP; i ++){

    Ant = (ant *)llist_getbyId(readyQueue,i);
    Ant->burstTime = i + 2;
    burstTime[i] = Ant->burstTime;
    temp[i] = burstTime[i];
  }

  for (int i = 0; i < NOP; i++){
    arrivalTime[i] = arrivalCounter;
    arrivalCounter++;
  }


  for(sum = 0, i = 0; y != 0;) {  

    if(temp[i] <= quantum && temp[i] > 0)   
  {  
      sum = sum + temp[i];  
      temp[i] = 0;  
      count=1;  
      }

      else if(temp[i] > 0)  
      {  
          temp[i] = temp[i] - quantum;  
          sum = sum + quantum;    
      }  
      if(temp[i]==0 && count==1)  
      {  
          y--;
          Ant = (ant *)llist_getbyId(readyQueue,i);
          waitingTime = waitingTime + sum- arrivalTime[i]- burstTime[i];  
          turnAroundTime = turnAroundTime + sum - arrivalTime[i];  
          count =0;     
      }  
      if(i==NOP-1)  
      {  
          i=0;  
      }  
      else if(arrivalTime[i+1] <= sum)  
      {  
          i++;  
      }  
      else  
      {  
          i=0;  
      }  
  }  

  return readyQueue;

}


int SJF_Scheduler(llist *Up, llist *Down, channel *Channel,cell map[24][46]){

  int getAntFrom = 0;
  llist* UpSort;
  llist* DownSort;
  ant *Ant;
  float accum = Channel->accumulator;
  struct timeval toc;

  gettimeofday(&toc, NULL);
  double time_Dif = (double)(toc.tv_sec - Channel->tic);
  accum += time_Dif;
  Channel->accumulator = accum;

  if(Channel->algorithm == 0){
    getAntFrom = Equidad(Up,Down,Channel,Channel->w_value );
  }
  if(Channel->algorithm == 1){
    getAntFrom = Letrero_Algorithm(Channel,Channel->m_time, Channel->n_time);
  }  
  if(Channel->algorithm == 2){
    getAntFrom = Tico_Algorithm(Up,Down,Channel);
  }

  //Sorting the queue
  if(getAntFrom){
    if(llist_getSize(Up) == 0){
      return 0;
    }
    UpSort = SJF_SortingAlgorithm(Up);
    Ant = (ant *)llist_getbyId(UpSort,0);
    if(Ant->posi == 9 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedDown = 0;
      Ant -> posi += 2;
    }
  } else{
    if(llist_getSize(Down) == 0){
      return 0;
    }
    DownSort = SJF_SortingAlgorithm(Down);
    Ant = (ant *)llist_getbyId(DownSort,0);
    if(Ant->posi == 13 && Ant->move == 0){
      sleep(1);
      map[Ant->posi][Ant->posj].usedUp = 0;
      Ant -> posi -= 2;
    }
  }


  if(Ant->move){
    return 0;
  }

  double wait = (double)Channel->length / (double)Ant->velocity;
  usleep(wait * 1000000);

  if(getAntFrom){
    Ant -> posi += 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Up,0);
    }
  } else{
    Ant -> posi -= 2;
    Ant -> move = 1;
    if(Ant->isAlive){
      llist_delById(Down,0);
    }
  }
  return 0;

}



dataScheduler *createDataScheduler(llist * Up, llist * Down,channel *Channel){
  dataScheduler *out= malloc(sizeof(dataScheduler));
  out->Up = Up;
  out->Down = Down;
  out->Channel = Channel;
  return out;
}
