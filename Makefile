CC=gcc
CF=-Wall -Wextra -pedantic
OUT=bin

run: zig_bin
	$(OUT)/lng $(args)

test: zig_test
	$(OUT)/test

pre:
	flex -L src/lexer.l
	bison -ld src/grammar.y
	
	sed -i 's/grammar.tab.h/parser.h/g' grammar.tab.c
	mv lex.yy.c src/lexer.c
	mv grammar.tab.c src/parser.c
	mv grammar.tab.h src/parser.h
	
out_dir: 
	mkdir -p $(OUT)

build: out_dir
	$(CC) $(CF) src/*.c -o $(OUT)/lng

build_print: out_dir
	$(CC) $(CF) -DPRINT_TABLE -DPRINT_TREE src/*.c -o $(OUT)/lng
