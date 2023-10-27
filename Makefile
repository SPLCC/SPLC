CC=gcc
FLEX=flex
BISON=bison

src_files := tmp/syntax.tab.c tmp/lex.yy.c src/lut.c src/ast.c src/spldefs.c src/utils.c
spl_preprocessor_out=spl_preprocessor
spl_parser_out=spl_parser

splc: spl_preprocessor spl_parser
	@echo "splc not implemented yet"

prepare: 
	@mkdir -p bin
	@mkdir -p tmp

spl_preprocessor:
	@echo "spl_preprocessor not implemented yet"

spl_parser: prepare
	$(BISON) --header=tmp/syntax.tab.h --output=tmp/syntax.tab.c src/syntax.y --report all
	$(FLEX) --outfile=tmp/lex.yy.c src/lex.l
	$(CC) -o bin/$(spl_parser_out) $(src_files) -Iinclude -Itmp -lfl -ly
	@chmod +x bin/$(spl_parser_out)

clean:
	@rm -rf bin/ tmp/
 
.PHONY: splc
