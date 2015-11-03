src=./src
build=./build
OBJ=Kmeans.o utils.o main.o
INCLUDES=
CC=g++
CCFLAGS=-Wall -std=c++11 -g
OBJFLAGS=-c
LDLIBS=-fopenmp
DFLAGS=

$(VERBOSE).SILENT:

all: ref
	rm -f *.o

ref.o: $(src)/1.ref/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) $(src)/1.ref/*.cpp

ref: ref.o $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o $(build)/ref

align.o: $(src)/2.align/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) $(src)/2.align/*.cpp

align: align.o $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o $(build)/align

vector.o: $(src)/3.vector/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) $(src)/3.vector/*.cpp

vector: vector.o $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o $(build)/vector

base.o: $(src)/4.base/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) $(src)/4.base/*.cpp

base: base.o $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o $(build)/base

clean:
	rm -f *.o ./build/*
