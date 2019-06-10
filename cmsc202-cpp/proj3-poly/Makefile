BCPPFLAGS = -ansi -Wall

proj3: poly.o node.o test.o
	g++ poly.o node.o test.o -o proj3

poly.o: Poly.cpp Poly.h
	g++ -c Poly.cpp -o poly.o

node.o: Node.cpp Node.h
	g++ -c Node.cpp -o node.o

test.o: Test.cpp
	g++ -c Test.cpp -o test.o