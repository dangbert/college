CFLAGS=-m32 -g -ansi -Wall

TARGET=a.out
MAIN=main5

# Modify following to the list of component object files
OBJS=frac_heap.o ${MAIN}.o

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${TARGET}

#main5.out: main5.o frac_heap.o
#	gcc -m32 $^ -o $@


clean:
	rm -f *.o *.out
	rm -f tmp.txt

run:
	./a.out

test:
	make clean
	clear
	make
	./a.out > tmp.txt
	vim -d tmp.txt $(MAIN).txt
