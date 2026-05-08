#!/bin/bash

INCLUDE_DIR="include"
SRC_DIR="src"

HEADER_EXTS=("h" "hpp" "hh")
SOURCE_EXTS=("c" "cpp" "cc" "cxx")

total_lines=0
header_count=0
source_count=0

count_file_lines () {
    wc -l < "$1"
}

is_header () {
    for ext in "${HEADER_EXTS[@]}"; do
        [[ "$1" == *.$ext ]] && return 0
    done
    return 1
}

is_source () {
    for ext in "${SOURCE_EXTS[@]}"; do
        [[ "$1" == *.$ext ]] && return 0
    done
    return 1
}

scan_dir () {
    local dir="$1"

    if [ ! -d "$dir" ]; then
        return
    fi

    while IFS= read -r file; do
        if is_header "$file"; then
            header_count=$((header_count + 1))
            lines=$(count_file_lines "$file")
            total_lines=$((total_lines + lines))

        elif is_source "$file"; then
            source_count=$((source_count + 1))
            lines=$(count_file_lines "$file")
            total_lines=$((total_lines + lines))
        fi

    done < <(find "$dir" -type f)
}

scan_dir "$INCLUDE_DIR"
scan_dir "$SRC_DIR"

total_files=$((header_count + source_count))

echo "===== CODE STATS ====="
echo "Total lines of code : $total_lines"
echo "Header files        : $header_count"
echo "Source files        : $source_count"
echo "Total files         : $total_files"