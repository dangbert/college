hw4: hw4.c
	gcc --std=c99 -Wall -O2 -o hw4 hw4.c hw4_test.c -pthread
#gcc --std=c99 -g -Wall -o hw4 hw4.c hw4_test.c -pthread

my: hw4.c my_test.c
	gcc --std=c99 -Wall -O2 -o hw4 hw4.c my_test.c -pthread
#gcc --std=c99 -g -Wall -o hw4 hw4.c my_test.c -pthread

.PHONY: run
run:
	make hw4
	./hw4 421

.PHONY: clean
clean:
	rm -f hw4

.PHONY: test
test:
	rm -f out.txt && make && ./hw4 421 > out.txt; vim -d *.txt
