#!/usr/bin/sh
find -type f -name '*.[hc]'|awk -F '\n' '{ system("clang-format -i " $0) }'
