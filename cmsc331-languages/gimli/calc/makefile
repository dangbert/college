calc331:	y.tab.c lex.yy.c
	gcc lex.yy.c y.tab.c -lm -o calc331
y.tab.c:	calc331.y
	yacc -d calc331.y
lex.yy.c:	calc331.lex
	lex calc331.lex
clean:	
	rm lex.yy.c y.tab.c y.tab.h calc331

