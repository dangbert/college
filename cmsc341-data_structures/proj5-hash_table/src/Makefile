FLAGS = -Wall
DRIVER = myDriver

all: Driver.o Pinball.o
	g++ $(FLAGS) Driver.o Pinball.o -o a.out

Driver.o: $(DRIVER).cpp words.h Pinball.h
	g++ $(FLAGS) -c $(DRIVER).cpp -o Driver.o

Pinball.o: Pinball.cpp Pinball.h
	g++ $(FLAGS) -c Pinball.cpp


run:
	./a.out

test:
	clear
	make clean
	make
#valgrind --leak-check=full ./a.out
	./a.out

save:
	scp -r . end1@gl.umbc.edu:proj5/src

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	rm -f temp.txt
