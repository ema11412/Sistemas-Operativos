.PHONY: clean all

APP_PROCESS=libprocess
LIB_PROCESS=process
OBJ_PROCESS=$(LIB_PROCESS).o
SRC_PROCESS=$(LIB_PROCESS).c
PROCESS=$(APP_PROCESS).a
INCLUDE_PROCESS=../include/process.h

APP_PPM=libppm
LIB_PPM=ppm
OBJ_PPM=$(LIB_PPM).o
SRC_PPM=$(LIB_PPM).c
PPM=$(APP_PPM).a
INCLUDE_PPM=../include/ppm.h

APP_PRINT=libprint
LIB_PRINT=print
OBJ_PRINT=$(LIB_PRINT).o
SRC_PRINT=$(LIB_PRINT).c
PRINT=$(APP_PRINT).a
INCLUDE_PRINT=../include/print.h

CC=gcc
MAKELIB=ar rv
RANLIB=ranlib

all: $(PROCESS) $(PPM) $(PRINT) clean

$(PROCESS): $(OBJ_PROCESS)
		$(MAKELIB) $(PROCESS) $(OBJ_PROCESS) && $(RANLIB) $(PROCESS)

$(OBJ_PROCESS): $(SRC_PROCESS) $(INCLUDE_PROCESS)
		$(CC) -c $(SRC_PROCESS) -I../include

$(PPM): $(OBJ_PPM)
		$(MAKELIB) $(PPM) $(OBJ_PPM) && $(RANLIB) $(PPM)

$(OBJ_PPM): $(SRC_PPM) $(INCLUDE_PPM)
		$(CC) -c $(SRC_PPM) -I../include

$(PRINT): $(OBJ_PRINT)
		$(MAKELIB) $(PRINT) $(OBJ_PRINT) && $(RANLIB) $(PRINT)

$(OBJ_PRINT): $(SRC_PRINT) $(INCLUDE_PRINT)
		$(CC) -c $(SRC_PRINT)

clean:
		rm  *.o