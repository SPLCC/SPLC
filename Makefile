FLEX = flex
BISON = bison
CC = gcc
CCFLAGS = -lfl -ly -Wall

.PHONY: splc debug syntax.y lex.yy.c check_syntax

all: clean splc

# ===================== Debug =====================
debug: CCFLAGS += -DDEBUG -g
debug: all

# ================= Dependencies =================
tmp/syntax.tab.c: src/syntax.y
	@mkdir -p tmp
	$(BISON) --header=tmp/syntax.tab.h --output=tmp/syntax.tab.c src/syntax.y --report all

syntax.y: tmp/syntax.tab.c

tmp/lex.yy.c: src/lex.l
	@mkdir -p tmp
	$(FLEX) --outfile=tmp/lex.yy.c --header-file=tmp/lex.yy.h src/lex.l

lex.yy.c: tmp/lex.yy.c

# ===================== splc =====================
splc_out=splc
splc_src_files := tmp/lex.yy.c tmp/syntax.tab.c $(wildcard src/*.c)
splc_includes = -Iinclude/ -Itmp/

bin/splc: $(splc_src_files)
	@mkdir -p bin
	@mkdir -p tmp
	$(CC) -o bin/$(splc_out) $(splc_src_files) $(splc_includes) $(CCFLAGS)
	@chmod +x bin/$(splc_out)

splc: bin/splc

check_syntax: src/syntax.y
	$(BISON) -d src/syntax.y -Wcex -v

# ===================== Other =====================
clean:
	@rm -rf bin/ tmp/