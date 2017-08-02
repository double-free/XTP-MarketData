###########################################
#Makefile for simple programs
###########################################
INC=
LIBPATH=$$HOME/Lib/XTP
LIB=-lxtpquoteapiD -lpthread
CC=g++ -std=c++14
# display all warnings
CC_FLAG=-Wall -g -O2

PRG=MarketMachine
OBJ=main.o xtpmdspi.o ringbuffer.o ThreadPool/thread_pool.o

$(PRG):$(OBJ)
	$(CC) $(INC) -o $@ $(OBJ) -L$(LIBPATH) $(LIB)

.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(PRG)
