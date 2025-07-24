#!/bin/bash
set -e

# Determine script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"

# If run from /test, adjust PROJECT_ROOT
if [[ $(basename "$PWD") == "test" ]]; then
  PROJECT_ROOT="$(dirname "$PWD")"
  cd "$PROJECT_ROOT"
fi

# Ensure build directory exists
mkdir -p build

# Compile using the real include path for accurate error locations
gcc -std=c99 -Wall -Wextra -Iinclude -o build/tests test/tests.c -g

echo "Compilation successful."
