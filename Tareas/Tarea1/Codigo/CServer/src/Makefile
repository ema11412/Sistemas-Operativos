APP=ImageServer
CC=gcc
ROUTE=../bin/
FLAGS=-L../lib -I../include -lpthread -lutilities -limages `pkg-config --cflags --libs MagickWand`
BIN=$(ROUTE)$(APP)
SRC1=main.c
SRC2=server.c

all: 
		$(CC) -o $(BIN) $(SRC1) $(SRC2)  $(FLAGS)
