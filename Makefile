# Makefile for demo programs for Lecture 8
# Networks
# We also need a queue and a min priority queue for this lecture

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

EXECUTABLES= Main$(EXECEXT)

all : $(EXECUTABLES)

linkedListQueue.o :	linkedListQueue.c abstractQueue.h 
	gcc -c linkedListQueue.c

minPriorityQueue.o :	minPriorityQueue.c minPriorityQueue.h
	gcc -c minPriorityQueue.c

hospital.o :	hospital.c hospital.h abstractNetwork.h
	gcc -c hospital.c

Inform.o :	Inform.c Inform.h hospital.h abstractNetwork.h
	gcc -c Inform.c

linkedListNetwork.o :	linkedListNetwork.c abstractNetwork.h abstractQueue.h minPriorityQueue.h
	gcc -c linkedListNetwork.c

AddMap.o :	AddMap.c abstractNetwork.h
	gcc -c AddMap.c

Main.o :	Main.c AddMap.h hospital.h Inform.h
	gcc -c Main.c

Main$(EXECEXT) : Main.o linkedListNetwork.o linkedListQueue.o minPriorityQueue.o hospital.o AddMap.o Inform.o
	gcc -o Main$(EXECEXT) Main.o linkedListNetwork.o linkedListQueue.o minPriorityQueue.o hospital.o AddMap.o Inform.o

clean : 
	-rm *.o
	-rm $(EXECUTABLES) 

