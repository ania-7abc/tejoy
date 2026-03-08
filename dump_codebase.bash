#!/bin/bash
# Improved script to dump contents of C++ header/source files with additional options.

set -o pipefail

VERSION="1.1.0"

# Default settings
output_file=""
verbose=0
names_only=0
include_patterns=("*.hpp" "*.cpp")
exclude_patterns=("*/build/*")
paths=()

# Function to display help
show_help() {
    cat << EOF
Usage: $0 [OPTIONS] [PATH...]

Dump contents of C++ source/header files. If PATHs are provided, they are processed as:
  - Regular file: included regardless of extension.
  - Directory: recursively searched for files matching include patterns (default: *.hpp, *.cpp).
If no PATH is given, the current directory (.) is used.

Options:
  -h, --help                 Show this help message and exit.
  --version                  Show version information and exit.
  -o, --output FILE          Write output to FILE instead of stdout.
  -v, --verbose              Print verbose information to stderr.
  -e, --exclude PATTERN      Exclude paths matching PATTERN (can be repeated).
  -i, --include PATTERN      Include files matching PATTERN (can be repeated; default: *.hpp *.cpp).
  -n, --names-only           Print only file names (one per line), no contents.

Examples:
  $0                                # dump all .hpp/.cpp from current dir (excluding build)
  $0 -o out.txt src/                # dump files from src/ into out.txt
  $0 -e "*/test/*" main.cpp util/   # exclude test/ paths, include main.cpp and all from util/

EOF
}

# Function to show version
show_version() {
    echo "$0 version $VERSION"
}

# Parse command line arguments using getopt
ARGS=$(getopt -o ho:ve:i:n --long help,output:,verbose,exclude:,include:,names-only,no-header,version -n "$0" -- "$@")
if [ $? -ne 0 ]; then
    exit 1
fi
eval set -- "$ARGS"

while true; do
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        --version)
            show_version
            exit 0
            ;;
        -o|--output)
            output_file="$2"
            shift 2
            ;;
        -v|--verbose)
            verbose=1
            shift
            ;;
        -e|--exclude)
            exclude_patterns+=("$2")
            shift 2
            ;;
        -i|--include)
            include_patterns+=("$2")
            shift 2
            ;;
        -n|--names-only)
            names_only=1
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "Internal error!" >&2
            exit 1
            ;;
    esac
done

# Remaining arguments are paths
paths=("$@")
if [ ${#paths[@]} -eq 0 ]; then
    paths=(".")
fi

# If output file is specified, redirect stdout
if [ -n "$output_file" ]; then
    exec > "$output_file"
fi

# Create temporary file to store null-separated file list
tempfile=$(mktemp)
trap 'rm -f "$tempfile"' EXIT

# Build the list of files
for path in "${paths[@]}"; do
    if [[ -f "$path" ]]; then
        # Explicit file: add as is
        printf "%s\0" "$path" >> "$tempfile"
    elif [[ -d "$path" ]]; then
        # Directory: use find with include/exclude patterns
        # Build find arguments safely using arrays
        find_args=(find "$path" -type f)

        # Construct name condition (parenthesized OR of -name patterns)
        if [ ${#include_patterns[@]} -gt 0 ]; then
            name_cond=()
            for pat in "${include_patterns[@]}"; do
                name_cond+=(-o -name "$pat")
            done
            # Replace the first '-o' with '(' and add closing ')'
            name_cond[0]='('
            name_cond+=(')')
            find_args+=("${name_cond[@]}")
        fi

        # Add exclude patterns
        for pat in "${exclude_patterns[@]}"; do
            find_args+=(-not -path "$pat")
        done

        # Add -print0 and execute
        find_args+=(-print0)
        "${find_args[@]}" >> "$tempfile"
    else
        echo "Warning: $path is not a regular file or directory, skipping." >&2
    fi
done

# Sort the file list (null-separated)
if [[ -s "$tempfile" ]]; then
    sort -z -o "$tempfile" "$tempfile"
else
    echo "No files found." >&2
    exit 0
fi

# Verbose: show number of files
if [ "$verbose" -eq 1 ]; then
    # Count null-separated entries
    count=$(tr '\0' '\n' < "$tempfile" | wc -l)
    echo "Number of files: $count" >&2
fi

# Read and process files
while IFS= read -r -d '' file; do
    if [ "$names_only" -eq 1 ]; then
        # Just print the file name
        echo "$file"
    else
        # Print header if enabled
        echo "================================================================================"
        echo "FILE: $file"
        echo "================================================================================"
        # Dump file contents
        cat "$file"
        echo   # extra newline as in original
    fi
done < "$tempfile"