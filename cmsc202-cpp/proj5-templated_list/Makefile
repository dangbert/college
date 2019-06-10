#CPPFLAGS = -ansi -Wall

proj5: 	node.o list.o main.o
	g++ node.o list.o main.o -o proj5

node.o: node.cpp node.h
	g++ -c node.cpp

list.o: list.cpp list.h
	g++ -c list.cpp -o list.o

main.o: proj5.cpp list.cpp list.h
	g++ -c proj5.cpp -o main.o