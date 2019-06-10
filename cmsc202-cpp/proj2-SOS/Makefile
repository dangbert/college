CPPFLAGS = -ansi -Wall

proj2: mytest.o sos.o
	g++ mytest.o sos.o -o proj2

mytest.o: mytest.cpp sos.h
	g++ -c mytest.cpp -o mytest.o

sos.o: sos.cpp sos.h
	g++ -c sos.cpp -o sos.o