APP = Ants
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).c
LIB_UT = util/libutil.a
LIB_GRAPH = graphics/libgraphics.a
LIB_DATA = data/libdata.a
LIB_L_LIST = linkedlist/liblinkedlist.a
LIB_CETHREAD = CEthread/libCEthread.a

CC = gcc
CFLAGS = -Wall -lm `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lconfig

LIBS = $(LIB)/$(LIB_UT) $(LIB)/$(LIB_GRAPH) $(LIB)/$(LIB_DATA) $(LIB)/$(LIB_L_LIST) $(LIB)/$(LIB_CETHREAD)


ALL: APP 
 
APP: 
		$(CC) -o $(BIN)/$(APP) $(SRC) $(LIBS) -I $(INCLUDE) $(CFLAGS)