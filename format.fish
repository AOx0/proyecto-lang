#!/usr/bin/fish

set source_dirs src include tests 

for dir in $source_dirs
    echo "Formatting $dir"
    for file in (exa -1 ./$dir|grep -E '.*(\.c|\.h)')
        clang-format -i ./$dir/$file
    end
end