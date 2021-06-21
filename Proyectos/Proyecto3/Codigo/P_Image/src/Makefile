APP=p_image
ROUTE=../bin/
FLAGS=-L../lib -I../include -lppm -lprocess -lprint

BIN=$(ROUTE)$(APP)
SRC=$(APP).c

all: COMPILE

COMPILE:
		gcc -o $(BIN) $(SRC) $(FLAGS)