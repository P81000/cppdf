#!/bin/bash

set -e

BUILD_TYPE=${1:-Debug}

PROJECT_ROOT=$(git rev-parse --show-toplevel 2>/dev/null || realpath $(dirname $0)/..)
BUILD_DIR="$PROJECT_ROOT/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE "$PROJECT_ROOT"

make
