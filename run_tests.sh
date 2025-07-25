#!/bin/bash
set -e

# Clean and build the project using CMake
rm -rf build
mkdir -p build
cd build
cmake ..
make
cd ..

echo "Running tests..."
./build/tests
