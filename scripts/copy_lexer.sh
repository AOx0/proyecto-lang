#!/usr/bin/sh
cp ./src/lexer.l ./src/ddlexer.l
sed -i -r 's/return[ \t]+(.+);/puts("\1");/g' ./src/ddlexer.l
sed -i 's/lexer./ddlexer./g' ./src/ddlexer.l
