#!/bin/bash
find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/build/*" -print0 | sort -z | while IFS= read -r -d '' file; do
    echo "================================================================================
FILE: $file
================================================================================"
    cat "$file"
    echo
done
