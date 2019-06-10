FLAGS = -Wall

all: Country.o Continent.o Driver.o
	g++ $(FLAGS) Country.o Continent.o Driver.o -o Driver.out

Country.o: Country.cpp
	g++ $(FLAGS) -c Country.cpp

Continent.o: Continent.cpp
	g++ $(FLAGS) -c Continent.cpp

Driver.o: Driver.cpp
	g++ $(FLAGS) -c Driver.cpp

run: all
	./Driver.out

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
