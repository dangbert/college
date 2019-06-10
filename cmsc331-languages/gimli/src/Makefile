gimli:	y.tab.c lex.yy.c
	gcc lex.yy.c y.tab.c -lm -o gimli
y.tab.c:	gimli.y
	yacc -v -d gimli.y
lex.yy.c:	gimli.lex
	lex gimli.lex
clean:	
	rm -f lex.yy.c y.tab.c y.tab.h y.output gimli

