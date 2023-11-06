#!/usr/bin/sh
rm -rf ./out

# Remove generated files by flex & bison
rm -rf src/parser.c src/lexer.h src/lexer.c src/parser.h

# Remove dummy flex
rm -rf ./src/ddlexer.l ./src/ddlexer.h ./src/ddlexer.c

# Remove zig stuff
# rm -rf ./zig-cache/ ./zig-out/
