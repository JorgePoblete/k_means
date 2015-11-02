OBJ=Kmeans.o utils.o main.o
INCLUDES=
CC=g++
CCFLAGS=-Wall -std=c++11 -g
OBJFLAGS=-c
LDLIBS=-fopenmp
DFLAGS=

$(VERBOSE).SILENT:

all: kmeans
	rm -f *.o

Kmeans.o: Kmeans.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) Kmeans.cpp

utils.o: utils.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) utils.cpp
	
main.o: main.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(INCLUDES) main.cpp

kmeans: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o Kmeans

clean:
	rm -f *.o Kmeans
