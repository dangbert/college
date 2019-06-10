FLAGS = -Wall
TESTNUM = 1

all: Driver.o Heap.o MinHeap.o MaxHeap.o PinHit.o
	g++ $(FLAGS) Driver.o Heap.o MinHeap.o MaxHeap.o PinHit.o -o Driver.out

Driver.o: Driver.cpp
	g++ $(FLAGS) -c Driver.cpp

Heap.o: Heap.cpp
	g++ $(FLAGS) -c Heap.cpp

MinHeap.o: MinHeap.cpp
	g++ $(FLAGS) -c MinHeap.cpp

MaxHeap.o: MaxHeap.cpp
	g++ $(FLAGS) -c MaxHeap.cpp

PinHit.o: PinHit.cpp
	g++ $(FLAGS) -c PinHit.cpp

# run with make run INPUT=<input_filename> FLAG=--<max/min>                
# e.g.     make run INPUT=Input1.txt FLAG=--max
run:
	./Driver.out $(INPUT) $(FLAG)

test:
	clear
	make clean
	make
	valgrind ./Driver.out sample_input/Input2.txt --max

save:
	scp -r . end1@gl.umbc.edu:proj4/src

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	rm -f temp.txt
