#!/usr/bin/env sh
find . -type f -name '*.[hc]'|awk -F '\n' '{ print("Formatting: " $0); system("clang-format -i " $0) }'
