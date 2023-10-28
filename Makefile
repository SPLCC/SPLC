CC=gcc
FLEX=flex
BISON=bison

.PHONY: spl_preprocessor spl_parser splc


# ===================== Preprocessor =====================
spl_preprocessor_out=spl_preprocessor

splc: spl_preprocessor spl_parser
	@echo "splc not implemented yet"

spl_preprocessor: 
	@echo "spl_preprocessor not implemented yet"

# ===================== Parser =====================
spl_parser_out=spl_parser
parser_src_files := src/parser/main.c src/parser/syntax.y src/parser/lex.l
parser_compile_files := src/parser/main.c tmp/parser_syntax.tab.c tmp/parser_lex.yy.c src/parser/lut.c src/parser/ast.c src/parser/spldef.c src/parser/utils.c
parser_includes = -Iinclude/parser/ -Itmp/

spl_parser:
	@mkdir -p bin
	@mkdir -p tmp
	$(BISON) --header=tmp/parser_syntax.tab.h --output=tmp/parser_syntax.tab.c src/parser/syntax.y --report all
	$(FLEX) --outfile=tmp/parser_lex.yy.c src/parser/lex.l
	$(CC) -o bin/$(spl_parser_out) $(parser_compile_files) $(parser_includes) -lfl -ly
	@chmod +x bin/$(spl_parser_out)


# ===================== Other =====================
clean:
	@rm -rf bin/ tmp/