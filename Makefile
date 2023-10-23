CC=gcc
FLEX=flex
BISON=bison
test:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c -lfl -ly -o test.out

tflex:
	$(FLEX) lex.l
	$(CC) lex.yy.c -lfl -o tflex.out

clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
.PHONY: test
