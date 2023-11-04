OUT=./out
CF=-Wextra -Wall -pedantic -g -O0
AF=--analyze -Xanalyzer -analyzer-output=text
# CF=-Wextra -Wall -pedantic
CC=zig cc

all: build test
	@echo Done

build: outdir
	$(CC) $(CF) -o $(OUT)/main \
		lib/vector.c \
		lib/err.c \
		lib/str.c \
		lib/hashset.c \
		lib/grammar.c \
		src/main.c

build_test: outdir
	$(CC) $(CF) -o $(OUT)/test \
		lib/vector.c \
		lib/err.c \
		lib/str.c \
		lib/hashset.c \
		lib/grammar.c \
		tests/vec_test.c \
		tests/str_test.c \
		tests/main.c

test: build_test
	$(OUT)/test

analyze:
	$(CC) $(CF) $(AF) -c \
		lib/vector.c \
		lib/err.c \
		lib/str.c \
		lib/hashset.c \
		lib/grammar.c \
		src/main.c

outdir:
	mkdir -p $(OUT)

run: build
	$(OUT)/main $(args)

clean:
	git clean -f -d -x
	rm -rf $(OUT)
