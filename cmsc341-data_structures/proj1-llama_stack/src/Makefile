FLAGS = -Wall
DRIVER = driver3

all: LlamaNode.o Llama.o Driver.o OvCoInt.o
	g++ $(FLAGS) LlamaNode.o Llama.o Driver.o OvCoInt.o -o a.out

LlamaNode.o: LlamaNode.cpp
	g++ $(FLAGS) -c LlamaNode.cpp

Llama.o: Llama.cpp
	g++ $(FLAGS) -c Llama.cpp

OvCoInt.o: OvCoInt.cpp
	g++ $(FLAGS) -c OvCoInt.cpp -o OvCoInt.o

Driver.o: $(DRIVER).cpp
	g++ $(FLAGS) -c $(DRIVER).cpp -o Driver.o

#run: all
run:
	make clean
	clear
	make
	./a.out

save:
	scp -r . end1@gl.umbc.edu:proj1/src

diff:
	make clean
	clear
	make
	./a.out > temp.txt 2>&1
	vim -d temp.txt ../expected_outputs/$(DRIVER).txt


clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	rm -f temp.txt
