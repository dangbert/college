1: candidate.c
	gcc --std=c99 -Wall -O2 -o candidate candidate.c
2: ballot.c
	gcc --std=c99 -Wall -O2 -o ballot ballot.c

.PHONY: run1
run1:
	make && ./candidate 'George Burdell'
#./candidate 'George Burdell' 2> >(xxd)

.PHONY: run2
run2:
	make && ./ballot
#sudo ./hw1 0x4020

.PHONY: clean
clean:
	rm -f candidate ballot
