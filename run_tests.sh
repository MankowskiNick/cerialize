#!/bin/bash
set -e

# Build the project using the existing build script
./build.sh

echo "Running tests..."
./build/tests
