#
# Leave the following lines alone!!!

LDFLAGS=-g -melf_i386
%.o: %.asm
	nasm -g -f elf -l $*.lst $< -o $@

#
# End of provided rules
#

# Modify following to the name of your linked program
TARGET=main3.out

# Modify following to the list of component object files
OBJS=mytest3.o prt_dec.o

#
# Do not modify remainder of this Makefile
#
${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} ${OBJS} -o ${TARGET}

run:
	./${TARGET}

clean:
	rm -f *.o
	rm -f *.lst
	rm -f *.out
