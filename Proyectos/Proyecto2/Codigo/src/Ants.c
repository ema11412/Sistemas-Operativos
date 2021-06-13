#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <util.h>
#include <graphics.h>
#include <data.h>
#include <CEthread.h>

//Constants
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 720
#define TILE_SIZE 30
int xTiles = SCREEN_WIDTH / TILE_SIZE + 1;
int yTiles = SCREEN_HEIGHT / TILE_SIZE;

//Variables globales
cell map[24][46]; //
int Types[100];
int exitProgram = 0;
int modeManual = 0;
int useClip = 0;
int baseVel = 60;
int maxTemp = 100;
int toggleInfo = 0;
int autoGenCount = 0;

llist *hormigueroA;
llist *hormigueroB;

llist *eastUp, *eastDown;
llist *centerUp, *centerDown;
llist *westUp, *westDown;

CEthread_mutex_t lock, fifo_lock;

//Bringes
channel East;
channel Center;
channel West;

configs Config;

//Functions
int threadAnt();

int delAnt(ant Ant, int x, int y);

int scheduler(void *Param);

void addInOrder(llist *List, ant *Ant);

void verifRand(ant *Ant);

int generatorA();

int generatorB();

//Algorithms

int delAntList(llist *List, ant *Ant);

int killAnt(ant *Ant);

void insertIntoList(ant *Ant);

void stopAnt(ant *Ant);


int main(int args, char **argv) {

    srand((unsigned) time(NULL));
    //Initialize Mutex

    loadMap(map);

    if (CEmutex_init(&lock, NULL) != 0) {
        printf("\n Mutex init has failed\n");
        return 1;
    }
    if (CEmutex_init(&fifo_lock, NULL) != 0) {
        printf("\n Mutex init has failed\n");
        return 1;
    }
    if (getChannelData(&East, "../config/eastChannel.conf")) {
        return 1;
    }
    if (getChannelData(&Center, "../config/centerChannel.conf")) {
        return 1;
    }
    if (getChannelData(&West, "../config/westChannel.conf")) {
        return 1;
    }
    if (getConfigsData(&Config, "../config/generalConfig.conf")) {
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        if (i < Config.probTypeA) {
            Types[i] = 0;
        } else if (i < (Config.probTypeA + Config.probTypeB)) {
            Types[i] = 1;
        } else if (i < (Config.probTypeA + Config.probTypeB + Config.probTypeC)) {
            Types[i] = 2;
        }
    }



    //Starting SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    //Starding SDL_Image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    //Starding SDL_TTF
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    //show config Window
    if (configWindow(&baseVel, &maxTemp, &exitProgram, &modeManual)) {
        return 1;
    }

    hormigueroA = llist_create(NULL);
    hormigueroB = llist_create(NULL);
    //Colas
    eastUp = llist_create(NULL);
    eastDown = llist_create(NULL);
    centerUp = llist_create(NULL);
    centerDown = llist_create(NULL);
    westUp = llist_create(NULL);
    westDown = llist_create(NULL);

    CEthread_t thread1, thread2, ChannelL, ChannelC, ChannelR;
    if (modeManual) {
        printf("Se selecciono el modo manual\n");
    } else if (!modeManual) {
        CEthread_create(&thread1, NULL, &generatorA, NULL);
        CEthread_create(&thread2, NULL, &generatorB, NULL);
        printf("Se selecciono el modo Auto\n");
    }


    dataScheduler *west = createDataScheduler(westUp, westDown, &West);
    dataScheduler *east = createDataScheduler(eastUp, eastDown, &East);
    dataScheduler *center = createDataScheduler(centerUp, centerDown, &Center);
    CEthread_create(&ChannelL, NULL, &scheduler, (void *) west);
    CEthread_create(&ChannelC, NULL, &scheduler, (void *) center);
    CEthread_create(&ChannelR, NULL, &scheduler, (void *) east);
    //Opening a Window
    SDL_Window *win = SDL_CreateWindow("Crazy Ant's", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    //Creating a renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color fontColor = {0, 0, 0, 255};
//Load Textures & Fonts
    char *fondoPath = "../assets/images/antbg.png";
    char *roadPath = "../assets/images/tile2.png";
    char *biroadPath = "../assets/images/tile1.png";
    char *channelPath = "../assets/images/tilew.png";
    char *antPath = "../assets/images/ants.png";
    char *castle1Path = "../assets/images/base1.png";
    char *castle2Path = "../assets/images/base2.png";
    char *iconPath = "../assets/images/ant2.png";
    char *infoPath = "../assets/images/info.png";
    char *mainFontPath = "../assets/fonts/font.ttf";

    SDL_Texture *BG = loadTexture(fondoPath, ren);
    SDL_Texture *BiRoad = loadTexture(biroadPath, ren);
    SDL_Texture *Channel = loadTexture(channelPath, ren);
    SDL_Texture *Road = loadTexture(roadPath, ren);
    SDL_Texture *Ant = loadTexture(antPath, ren);
    SDL_Texture *Castle1 = loadTexture(castle1Path, ren);
    SDL_Texture *Castle2 = loadTexture(castle2Path, ren);
    SDL_Texture *Info = loadTexture(infoPath, ren);

    SDL_Surface *icon = IMG_Load(iconPath);
    SDL_SetWindowIcon(win, icon);

    SDL_RenderClear(ren);
    renderTexture(Info, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
    SDL_Delay(1000);

    //Texts
    SDL_Texture *castleA = renderText("Hormiguero 1", mainFontPath, fontColor, 14, ren);
    SDL_Texture *castleB = renderText("Hormiguero 2", mainFontPath, fontColor, 14, ren);


    //Setup the clips for Hormiguero A
    SDL_Rect clipsA[3][3];
    for (int i = 0; i < 3; i++) {
        loadClips(clipsA[i], 1, i + 1, 69, 70);
    }

    //Setup the clips for Hormiguero B
    SDL_Rect clipsB[3][3];
    for (int i = 0; i < 3; i++) {
        loadClips(clipsB[i], 2, i + 1, 69, 70);
    }

    int mouseX, mouseY;
    int animCounter = 0;
    SDL_Event e;
    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time

    while (!exitProgram) {
        //Event Handler
        while (SDL_PollEvent(&e)) {
            //If user closes the window
            if (e.type == SDL_QUIT) {
                exitProgram = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);

                for (int i = 0; i < llist_getSize(hormigueroA); i++) {
                    ant *DUT = (ant *) llist_getbyId(hormigueroA, i);
                    int y = DUT->posi;
                    int x = DUT->posj;
                    if (delAnt(*DUT, mouseX, mouseY)) {
                        DUT->isAlive = 0;
                        map[y][x].usedDown = 0;
                        break;
                    }
                }

                for (int i = 0; i < llist_getSize(hormigueroB); i++) {
                    ant *DUT = (ant *) llist_getbyId(hormigueroB, i);
                    int y = DUT->posi;
                    int x = DUT->posj;
                    if (delAnt(*DUT, mouseX, mouseY)) {
                        DUT->isAlive = 0;
                        map[y][x].usedUp = 0;
                        break;
                    }
                }
            }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_i) {
                    toggleInfo = toggleInfo ? 0 : 1;
                }
                if (modeManual) {
                    if (e.key.keysym.sym == SDLK_1) {
                        ant *Ant = createAntManual(baseVel, 1, 0);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        
                        llist_addLast(hormigueroA, Ant);
                    } else if (e.key.keysym.sym == SDLK_2) {
                        ant *Ant = createAntManual(baseVel, 1, 1);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        llist_addLast(hormigueroA, Ant);
                    } else if (e.key.keysym.sym == SDLK_3) {
                        ant *Ant = createAntManual(baseVel, 1, 2);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        llist_addLast(hormigueroA, Ant);
                    } else if (e.key.keysym.sym == SDLK_4) {
                        ant *Ant = createAntManual(baseVel, 0, 0);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        llist_addLast(hormigueroB, Ant);
                    } else if (e.key.keysym.sym == SDLK_5) {
                        ant *Ant = createAntManual(baseVel, 0, 1);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        llist_addLast(hormigueroB, Ant);
                    } else if (e.key.keysym.sym == SDLK_6) {
                        ant *Ant = createAntManual(baseVel, 0, 2);
                        CEthread_t *thread = malloc(sizeof(CEthread_t));
                        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
                        Ant->pid = thread->pid;
                        llist_addLast(hormigueroB, Ant);
                    }
                }
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        useClip = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the background
        renderTexture(BG, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        for (int i = 0; i < yTiles; i++) {
            for (int j = 0; j < xTiles; j++) {
                if (map[i][j].type == 1) {
                    renderTexture(Road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                } else if (map[i][j].type == 2) {
                    renderTexture(BiRoad, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                } else if (map[i][j].type == 3) {
                    renderTexture(Channel, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                }
            }
        }


        for (int i = 0; i < llist_getSize(hormigueroA); i++) {
            ant *temp = (ant *) llist_getbyId(hormigueroA, i);
            if (temp != NULL)
                renderTextureSheet(Ant, ren, temp->posj * TILE_SIZE - 5, temp->posi * TILE_SIZE, 25,
                                   &clipsA[temp->type][useClip]);
        }

        for (int i = 0; i < llist_getSize(hormigueroB); i++) {
            ant *temp = (ant *) llist_getbyId(hormigueroB, i);
            if (temp != NULL)
                renderTextureSheet(Ant, ren, temp->posj * TILE_SIZE + 10, temp->posi * TILE_SIZE, 25,
                                   &clipsB[temp->type][useClip]);
        }

        renderTexture(Castle1, ren, 10, 70, 170, 170);
        renderTextureFull(castleA, ren, 15, 260);
        renderTexture(Castle2, ren, SCREEN_WIDTH - 190, 500, 190, 170);
        renderTextureFull(castleB, ren, SCREEN_WIDTH - 200, 450);

        if (toggleInfo) {
            renderTexture(Info, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        //Update the screen
        SDL_RenderPresent(ren);

        SDL_Delay(17);

        animCounter += 1;
        if (animCounter == 15) {
            animCounter = 0;
            useClip += 1;
            if (useClip == 3)
                useClip = 0;
        }
    }


    llist_free(hormigueroA);
    llist_free(hormigueroB);

    SDL_DestroyTexture(BiRoad);
    SDL_DestroyTexture(Channel);
    SDL_DestroyTexture(Road);
    SDL_DestroyTexture(Ant);
    SDL_DestroyTexture(Castle1);
    SDL_DestroyTexture(Castle2);
    SDL_DestroyTexture(castleA);
    SDL_DestroyTexture(castleB);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    CEmutex_destroy(&lock);
    CEmutex_destroy(&fifo_lock);
    return 0;
}


int threadAnt(void *param) {

    ant *Ant = (ant *) param;
    struct timeval toc;

    while (Ant->isAlive) {
        CEmutex_lock(&lock);
        gettimeofday(&toc, NULL);

        double elapsed = ((double) toc.tv_sec - Ant->tic);
        if (elapsed > (double) maxTemp && Ant->type == 2) {
            Ant->isAlive = 0;
            if (Ant->direction == 'B') {
                map[Ant->posi][Ant->posj].usedDown = 0;
            } else {
                map[Ant->posi][Ant->posj].usedUp = 0;
            }
            CEmutex_unlock(&lock);
            break;
        }

        if (Ant->move)
            moveAnt(Ant, map);
        verifRand(Ant);
        insertIntoList(Ant);
        stopAnt(Ant);

        int i = Ant->posi;
        int j = Ant->posj;
        if ((i == 1 && j == 24) || (i == 23 && j == 20)) {
            Ant->isAlive = 0;
            if (Ant->direction == 'B') {
                map[Ant->posi][Ant->posj].usedDown = 0;
            } else {
                map[Ant->posi][Ant->posj].usedUp = 0;
            }
        }
        CEmutex_unlock(&lock);

        double wait = Ant->velocity;
        double newWait = 5 / wait * 10;
        usleep(newWait * 1000000); //microsec
        // velocity = 200 then sleep 1/4 sec (max)
        // velocity = 5 then sleep 10 sec (min)
    }
    killAnt(Ant);
    return 0;
}

int delAntList(llist *List, ant *Ant) {
    for (int i = 0; i < llist_getSize(List); i++) {
        ant *AUT = (ant *) llist_getbyId(List, i);
        if (AUT->pid == Ant->pid) {
            llist_delById(List, i);
            return 0;
        }
    }
    return 0;
}

int killAnt(ant *Ant) {
    delAntList(hormigueroA, Ant);
    delAntList(hormigueroB, Ant);
    delAntList(westUp, Ant);
    delAntList(westDown, Ant);
    delAntList(centerUp, Ant);
    delAntList(centerDown, Ant);
    delAntList(eastUp, Ant);
    delAntList(eastDown, Ant);
    return 0;
}

int delAnt(ant Ant, int x, int y) {
    SDL_Rect mouse;
    mouse.x = x;
    mouse.y = y;
    mouse.w = 1;
    mouse.h = 1;

    SDL_Rect al;
    al.x = Ant.posj * TILE_SIZE;
    al.y = Ant.posi * TILE_SIZE;
    al.w = TILE_SIZE;
    al.h = TILE_SIZE;

    if (SDL_HasIntersection(&mouse, &al))
        return 1;
    else
        return 0;
}


int generatorA() {
    while (1) {
        ant *Ant = createAnt(baseVel, 1, Types);
        CEthread_t *thread = malloc(sizeof(CEthread_t));
        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
        Ant->pid = thread->pid;
        llist_addLast(hormigueroA, Ant);

        double num = expRand(Config.mean);

        usleep(num * 2000000);

    }
    return 0;
}

int generatorB() {
    while (1) {
        ant *Ant = createAnt(baseVel, 0, Types);
        CEthread_t *thread = malloc(sizeof(CEthread_t));
        CEthread_create(thread, NULL, &threadAnt, (void *) Ant);
        Ant->pid = thread->pid;
        llist_addLast(hormigueroB, Ant);

        double num = expRand(Config.mean);

        usleep(num * 2000000);
    }
    return 0;
}

void insertIntoList(ant *Ant) {
    if (Ant->direction == 'B') {
        if (Ant->posi == 5 && Ant->posj == 21) {
            if (West.scheduler == 1) {
                addInOrder(westUp, Ant);
            } else {
                llist_addLast(westUp, Ant);
            }

        } else if (Ant->posi == 5 && Ant->posj == 23) {
            if (East.scheduler == 1) {
                addInOrder(eastUp, Ant);
            } else {
                llist_addLast(eastUp, Ant);
            }

        } else if (Ant->posi == 6 && Ant->posj == 22) {
            if (Center.scheduler == 1) {
                addInOrder(centerUp, Ant);
            } else {
                llist_addLast(centerUp, Ant);
            }

        }
    } else if (Ant->direction == 'A') {
        if (Ant->posi == 17 && Ant->posj == 21) {
            if (West.scheduler == 1) {
                addInOrder(westDown, Ant);
            } else {
                llist_addLast(westDown, Ant);
            }

        } else if (Ant->posi == 17 && Ant->posj == 23) {
            if (East.scheduler == 1) {
                addInOrder(eastDown, Ant);
            } else {
                llist_addLast(eastDown, Ant);
            }

        } else if (Ant->posi == 16 && Ant->posj == 22) {
            if (Center.scheduler == 1) {
                addInOrder(centerDown, Ant);
            } else {
                llist_addLast(centerDown, Ant);
            }
        }
    }
}

void stopAnt(ant *Ant) {
    int i = Ant->posi;
    int j = Ant->posj;

    if ((i == 9 && j == 11) && Ant->direction == 'B') {
        Ant->move = 0;
    } else if ((i == 9 && j == 22) && Ant->direction == 'B') {
        Ant->move = 0;
    } else if ((i == 9 && j == 33) && Ant->direction == 'B') {
        Ant->move = 0;
    } else if ((i == 13 && j == 11) && Ant->direction == 'A') {
        Ant->move = 0;
    } else if ((i == 13 && j == 22) && Ant->direction == 'A') {
        Ant->move = 0;
    } else if ((i == 13 && j == 33) && Ant->direction == 'A') {
        Ant->move = 0;
    }
}

int scheduler(void *Param) {
    dataScheduler *Data = (dataScheduler *) Param;
    int quantum = Config.quantum;
    while (1) {
        if (Data->Channel->scheduler == 0) {
            RR_Scheduler(Data->Up, Data->Down, Data->Channel, map, quantum);
        } else if (Data->Channel->scheduler == 1) {
            Priority(Data->Up, Data->Down, Data->Channel, map);
        } else if (Data->Channel->scheduler == 2) {
            SJF_Scheduler(Data->Up, Data->Down, Data->Channel, map);
        } else if (Data->Channel->scheduler == 3) {
            FIFO(Data->Up, Data->Down, Data->Channel, map);
        } else if (Data->Channel->scheduler == 4) {
            continue;
        }

        usleep(100000);
    }
    return 0;
}

void addInOrder(llist *List, ant *Ant) {
    int size = llist_getSize(List);
    ant *Ant2;
    if (size == 0) {
        llist_addLast(List, Ant);
    }
    for (int i = 0; i < size; i++) {
        Ant2 = (ant *) llist_getbyId(List, i);
        if (Ant->type > Ant2->type) {
            llist_addById(List, Ant, i);
            break;
        }
        if (i == size - 1) {
            llist_addLast(List, Ant);
            break;
        }
    }
}

void verifRand(ant *Ant) {
    int i = Ant->posi;
    int j = Ant->posj;
    if (i == 5 && j == 22 && Ant->direction == 'B') {
        Ant->move = 0;
        usleep(500000);
        int dir = rand() % 3;
        if (dir == 0 && llist_getSize(westUp) < 4) {
            map[i][j].usedDown = 0;
            Ant->posj -= 1;
            Ant->lmove = 4;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedDown = 1;
        } else if (dir == 1 && llist_getSize(centerUp) < 3) {
            map[i][j].usedDown = 0;
            Ant->posi += 1;
            Ant->lmove = 3;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedDown = 1;
        } else if (dir == 2 && llist_getSize(eastUp) < 4) {
            map[i][j].usedDown = 0;
            Ant->posj += 1;
            Ant->lmove = 2;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedDown = 1;
        }
    } else if (i == 17 && j == 22 && Ant->direction == 'A') {
        Ant->move = 0;
        usleep(500000);
        int dir = rand() % 3;
        if (dir == 0 && llist_getSize(westDown) < 4) {
            map[i][j].usedUp = 0;
            Ant->posj -= 1;
            Ant->lmove = 4;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedUp = 1;
        } else if (dir == 1 && llist_getSize(centerDown) < 3) {
            map[i][j].usedUp = 0;
            Ant->posi -= 1;
            Ant->lmove = 1;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedUp = 1;
        } else if (dir == 2 && llist_getSize(eastDown) < 4) {
            map[i][j].usedUp = 0;
            Ant->posj += 1;
            Ant->lmove = 2;
            Ant->move = 1;
            map[Ant->posi][Ant->posj].usedUp = 1;
        }
    }
}