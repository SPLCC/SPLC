CC=gcc
FLEX=flex
BISON=bison

.PHONY: splc

all: splc

# ===================== splc =====================
splc_out=splc
splc_src_files := src/main.c src/syntax.y src/lex.l
splc_compile_files := src/main.c tmp/syntax.tab.c tmp/lex.yy.c src/lut.c src/ast.c src/spldef.c src/utils.c
splc_includes = -Iinclude/ -Itmp/

splc:
	@mkdir -p bin
	@mkdir -p tmp
	$(BISON) --header=tmp/syntax.tab.h --output=tmp/syntax.tab.c src/syntax.y --report all
	$(FLEX) --outfile=tmp/lex.yy.c src/lex.l
	$(CC) -o bin/$(splc_out) $(splc_compile_files) $(splc_includes) -lfl -ly -Wall
	@chmod +x bin/$(splc_out)


# ===================== Other =====================
clean:
	@rm -rf bin/ tmp/