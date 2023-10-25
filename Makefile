CC=gcc
FLEX=flex
BISON=bison
test:
	$(BISON) -d syntax.y --report all
	$(FLEX) lex.l
	$(CC) syntax.tab.c treenode.c -lfl -ly -o test.out

clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out syntax.output
.PHONY: test