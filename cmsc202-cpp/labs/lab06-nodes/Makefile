CPPFLAGS = -ansi -Wall

lab6: lab6.o List.o
	g++ lab6.o List.o -o lab6

lab6.o: lab6.cpp List.h
	g++ -c lab6.cpp -o lab6.o

List.o: List.cpp List.h
	g++ -c List.cpp -o List.o