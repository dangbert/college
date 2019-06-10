CPPFLAGS = -ansi -Wall

test: test.o
		g++ test.o -o test

#Test.o: Test.cpp Test.h
test.o: test.cpp
		g++ -c test.cpp -o test.o


run:
	./test

clean:
	rm -f *.o test
