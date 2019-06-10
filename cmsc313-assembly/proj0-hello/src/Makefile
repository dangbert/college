#
# Leave the following lines alone!!!

LDFLAGS=-g -melf_i386
%.o: %.asm
	nasm -g -f elf -l $*.lst $< -o $@

#
# End of provided rules
#

# Modify following to the name of your linked program
TARGET=hello.out

# Modify following to the list of component object files
OBJS=hello.o

#
# Do not modify remainder of this Makefile
#
${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} ${OBJS} -o ${TARGET}
