#
# Leave the following lines alone!!!

CFLAGS=-ansi -Wall -g

#
# End of provided rules
#

# Modify following to the name of your linked program
TARGET=main7.out

# Modify following to the list of component object files
OBJS=main7.o AnimalClasses.o

#
# Do not modify remainder of this Makefile
#
${TARGET}: ${OBJS}
	gcc -ansi -Wall -g ${OBJS} -o $@


clean:
	rm -f *.o *.out

run:
	./main7.out
