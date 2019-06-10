CPPFLAGS = -ansi -Wall

labp: Rectangle.o Shape.o Triangle.o Lab9.o
	g++ Rectangle.o Shape.o Triangle.o Lab9.o -o lab9

Rectangle.o: Rectangle.cpp Rectangle.h
	g++ -c Rectangle.cpp -o Rectangle.o

Shape.o: Shape.cpp Shape.h
	g++ -c Shape.cpp -o Shape.o

Triangle.o: Triangle.cpp Triangle.h
	g++ -c Triangle.cpp -o Triangle.o

Lab9.o: Lab9.cpp
	g++ -c Lab9.cpp -o Lab9.o