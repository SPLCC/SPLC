CC=gcc
FLEX=flex
BISON=bison

# ===================== Preprocessor =====================
spl_preprocessor_out=spl_preprocessor

splc: bin/spl_preprocessor bin/spl_parser
	@echo "splc not implemented yet"

bin/spl_preprocessor: 
	@echo "spl_preprocessor not implemented yet"

spl_preprocessor: bin/spl_preprocessor

# ===================== Parser =====================
spl_parser_out=spl_parser
parser_src_files := src/parser/main.c src/parser/syntax.y src/parser/lex.l
parser_compile_files := src/parser/main.c tmp/parser_syntax.tab.c tmp/parser_lex.yy.c
parser_obj_files := tmp/obj/parser/lut.o tmp/obj/parser/ast.o tmp/obj/parser/spldef.o tmp/obj/parser/utils.o
parser_includes = -Iinclude/parser/ -Itmp/


tmp/obj/parser/lut.o: src/parser/lut.c
	@mkdir -p tmp/obj/parser
	$(CC) -o tmp/obj/parser/lut.o -c src/parser/lut.c $(parser_includes) -Wall

tmp/obj/parser/ast.o: src/parser/ast.c
	@mkdir -p tmp/obj/parser
	$(CC) -o tmp/obj/parser/ast.o -c src/parser/ast.c $(parser_includes) -Wall

tmp/obj/parser/spldef.o: src/parser/spldef.c
	@mkdir -p tmp/obj/parser
	$(CC) -o tmp/obj/parser/spldef.o -c src/parser/spldef.c $(parser_includes) -Wall

tmp/obj/parser/utils.o: src/parser/utils.c
	@mkdir -p tmp/obj/parser
	$(CC) -o tmp/obj/parser/utils.o -c src/parser/utils.c $(parser_includes) -Wall


bin/spl_parser: $(parser_obj_files) $(parser_src_files)
	@mkdir -p bin
	@mkdir -p tmp
	$(BISON) --header=tmp/parser_syntax.tab.h --output=tmp/parser_syntax.tab.c src/parser/syntax.y --report all
	$(FLEX) --outfile=tmp/parser_lex.yy.c src/parser/lex.l
	$(CC) -o bin/$(spl_parser_out) $(parser_compile_files) $(parser_obj_files) $(parser_includes) -lfl -ly
	@chmod +x bin/$(spl_parser_out)

spl_parser: bin/spl_parser

clean:
	@rm -rf bin/ tmp/
 
.PHONY: splc
