CFLAGS=-m32 -g -ansi -Wall

# or run with: make MAIN=test1
MAIN = main6

main6.out: $(MAIN).o frac_heap.o
	gcc -m32 $^ -o $@

clean:
	rm -f *.o *.out *.lst

run:
	./main6.out
