lextest: driver.o tokens.o lex.yy.o errormsg.o util.o
	cc -g -o lextest driver.o tokens.o lex.yy.o errormsg.o util.o

driver.o: driver.c tokens.h errormsg.h util.h
	cc -g -c driver.c

tokens.o: tokens.c tokens.h util.h
	cc -g -c tokens.c

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c tokens.h errormsg.h util.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.lex
	lex tiger.lex

util.o: util.c util.h
	cc -g -c util.c

clean: 
	rm -f lextest a.out util.o driver.o tokens.o lex.yy.o lex.yy.c errormsg.o
