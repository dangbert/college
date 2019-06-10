cPPFCPPFLAGS = -ansi -Wall

lab5: lab5.o Complex.o
	g++ lab5.o Complex.o -o lab5

lab5.o: lab5.cpp Complex.h
	g++ -c lab5.cpp -o lab5.o

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp -o Complex.o