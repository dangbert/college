CPPFLAGS = -ansi -Wall

lab10: Complex.o lab10.o
	g++ Complex.o lab10.o -o lab10

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp -o Complex.o

lab10.o: lab10.cpp
	g++ -c lab10.cpp -o lab10.o