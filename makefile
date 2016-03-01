src=./src
build=./build
OBJ=Kmeans.o utils.o main.o
INCLUDES=
CC=icpc
CCFLAGS=-Wall -std=c++11 -g
OPT=
OPTFLAGS=
OBJFLAGS=-c
LDLIBS=-fopenmp
DFLAGS=

$(VERBOSE).SILENT:

all: ref
	rm -f *.o

ref.o: $(src)/1.ref/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/1.ref/*.cpp

ref: ref.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/ref

em.o: $(src)/2.em/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/2.em/*.cpp

em: em.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/em

align.o: $(src)/3.align/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/3.align/*.cpp

align: align.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/align

vector.o: $(src)/4.vector/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/4.vector/*.cpp

vector: vector.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/vector

parallel.o: $(src)/5.parallel/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/5.parallel/*.cpp

parallel: parallel.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/parallel


task.o: $(src)/6.task/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/6.task/*.cpp

task: task.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/task

offload.o: $(src)/7.offload/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/7.offload/*.cpp

offload: offload.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/offload

mini-batch.o: $(src)/8.mini-batch/*
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) $(src)/8.mini-batch/*.cpp

mini-batch: mini-batch.o
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(OPT) $(OPTFLAGS) $(INCLUDES) -o $(build)/mini-batch

clean:
	rm -f *.o* ./build/*
