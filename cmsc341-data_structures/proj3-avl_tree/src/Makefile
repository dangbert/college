FLAGS = -Wall
DRIVER = Driver
TESTNUM = 1

all: Driver.o AVLForest.o AVLTree.o AVLNode.o
	g++ $(FLAGS) Driver.o AVLForest.o AVLTree.o AVLNode.o -o Driver.out

Driver.o: $(DRIVER).cpp
	g++ $(FLAGS) -c $(DRIVER).cpp -o Driver.o

AVLForest.o: AVLForest.cpp
	g++ $(FLAGS) -c AVLForest.cpp

AVLTree.o: AVLTree.cpp
	g++ $(FLAGS) -c AVLTree.cpp

AVLNode.o: AVLNode.cpp
	g++ $(FLAGS) -c AVLNode.cpp


# run with make run INPUT=<input_filename> RANGE=<range_filename>
# e.g.     make run INPUT=input.txt RANGE=range.txt
run:
	./Driver.out $(INPUT) $(RANGE)

test:
	clear
	make clean
	make
	#valgrind --leak-check=full ./Driver.out sample_input/input$(TESTNUM).txt sample_input/range$(TESTNUM).txt > temp.txt 2>&1
	./Driver.out sample_input/input$(TESTNUM).txt sample_input/range$(TESTNUM).txt > temp.txt 2>&1
	vim temp.txt

save:
	scp -r . end1@gl.umbc.edu:proj3/src

diff:
	make clean
	clear
	make
	./Driver.out sample_input/input$(TESTNUM).txt sample_input/range$(TESTNUM).txt > temp.txt 2>&1
	vim -d temp.txt sample_output/output$(TESTNUM).txt

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	rm -f temp.txt
