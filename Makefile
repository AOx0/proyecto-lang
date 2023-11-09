CC=zig cc
CF=-Wall -Wextra -pedantic
OUT=zig-out/bin

run: zig_bin
	$(OUT)/lng $(args)

test: zig_test
	$(OUT)/test

pre:
	flex -Ld src/lexer.l
	bison -ldt src/grammar.y
	
	mv lex.yy.c src/lexer.c
	mv grammar.tab.c src/parser.c
	mv grammar.tab.h src/parser.h
	
out_dir: 
	mkdir -p $(OUT)

zig_lib: out_dir
	zig build-lib -cflags $(CF) -- \
		src/str.c \
		src/vector.c \
		src/hashset.c \
		src/parser.c \
		src/lexer.c \
		-lc --cache-dir zig-cache \
		--global-cache-dir $(HOME)/.cache/zig \
		--name lnglib -static
	mv liblnglib.a $(OUT)/liblnglib.a

zig_test: zig_lib
	zig build-exe $(OUT)/liblnglib.a -cflags $(CF) -- \
		test/main.c \
		test/str_test.c \
		test/vec_test.c \
		test/hash_test.c \
		-lc --cache-dir zig-cache \
		--global-cache-dir $(HOME)/.cache/zig \
		--name tmptest
	mv tmptest $(OUT)/test
	
zig_bin: zig_lib
	zig build-exe $(OUT)/liblnglib.a -cflags $(CF) -- \
		src/main.c \
		-lc \
		--cache-dir zig-cache \
		--global-cache-dir $(HOME)/.cache/zig \
		--name tmplng
	mv tmplng $(OUT)/lng

build_bin: build_lib
	mkdir -p zig-out/bin
	$(CC) $(CF) -o zig-out/bin/lng \
		zig-out/bin/lng.o
		src/main.c

build_lib:
	mkdir -p zig-out/bin
	$(CC) $(CF) -o zig-out/bin/lng.o \
		src/str.c \
		src/vector.c \
		src/hashset.c \
		src/parser.c \
		src/lexer.c 