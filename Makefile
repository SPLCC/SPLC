CC=gcc
FLEX=flex
BISON=bison
spl_preprocessor_out=spl_preprocessor
spl_parser_out=spl_parser

spl_preprocessor:
	@echo "spl_preprocessor not implemented yet"

spl_parser:
	@mkdir bin
	@mkdir tmp
	$(BISON) --header=tmp/syntax.tab.h --output=tmp/syntax.tab.c src/syntax.y --report all
	$(FLEX) --outfile=tmp/lex.yy.c src/lex.l
	$(CC) -o bin/$(spl_parser_out) tmp/syntax.tab.c src/treenode.c -Itmp -lfl -ly
	@chmod +x bin/$(spl_parser_out)

splc: spl_preprocessor spl_parser
	@echo "splc not implemented yet"

clean:
	@rm -rf bin/
 
.PHONY: splc
