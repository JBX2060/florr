#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status
set -u  # Treat unset variables as an error

# Function to print error messages
error() {
    echo "Error: $1" >&2
    exit 1
}

# Determine the operating system
OS=$(uname -s)

# Determine the number of CPU cores for parallel make
if command -v nproc >/dev/null 2>&1; then
    NUM_CORES=$(nproc)
elif sysctl -n hw.ncpu >/dev/null 2>&1; then
    NUM_CORES=$(sysctl -n hw.ncpu)
else
    NUM_CORES=1
fi

# Store the project root directory
PROJECT_ROOT="$(pwd)"

if [ "$OS" = "Darwin" ]; then
    # macOS specific build
    # Paths to dependencies
    LIBWEBSOCKETS_INCLUDE="/opt/homebrew/opt/libwebsockets/include"
    LIBWEBSOCKETS_LIB="/opt/homebrew/opt/libwebsockets/lib"
    OPENSSL_INCLUDE="/opt/homebrew/opt/openssl@3/include"
    OPENSSL_LIB="/opt/homebrew/opt/openssl@3/lib"

    # Check if dependency paths exist
    [ -d "$LIBWEBSOCKETS_INCLUDE" ] || error "Libwebsockets include directory not found at $LIBWEBSOCKETS_INCLUDE"
    [ -d "$LIBWEBSOCKETS_LIB" ] || error "Libwebsockets library directory not found at $LIBWEBSOCKETS_LIB"
    [ -d "$OPENSSL_INCLUDE" ] || error "OpenSSL include directory not found at $OPENSSL_INCLUDE"
    [ -d "$OPENSSL_LIB" ] || error "OpenSSL library directory not found at $OPENSSL_LIB"

    # Build Client
    echo "Building Client on macOS..."
    mkdir -p "$PROJECT_ROOT/Client/build"
    cd "$PROJECT_ROOT/Client/build"

    cmake -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DWASM_BUILD=1 "$PROJECT_ROOT/Client"
    make -j"$NUM_CORES"
    cd "$PROJECT_ROOT"

    # Build Server
    echo "Building Server on macOS..."
    mkdir -p "$PROJECT_ROOT/Server/build"
    cd "$PROJECT_ROOT/Server/build"

    cmake \
        -DCMAKE_C_FLAGS="-I$LIBWEBSOCKETS_INCLUDE -I$OPENSSL_INCLUDE" \
        -DCMAKE_EXE_LINKER_FLAGS="-L$LIBWEBSOCKETS_LIB -lwebsockets -L$OPENSSL_LIB -lssl -lcrypto" \
        -DCMAKE_C_COMPILER=gcc-13 \
        -DCMAKE_CXX_COMPILER=g++-13 \
        "$PROJECT_ROOT/Server"
    make -j"$NUM_CORES"
    cd "$PROJECT_ROOT"

elif [ "$OS" = "Linux" ]; then
    # Ubuntu specific build

    # Build Client
    echo "Building Client on Ubuntu..."
    mkdir -p "$PROJECT_ROOT/Client/build"
    cd "$PROJECT_ROOT/Client/build"

    cmake "$PROJECT_ROOT/Client" -DDEBUG_BUILD=0 -DWASM_BUILD=1
    make -j"$NUM_CORES"
    cd "$PROJECT_ROOT"

    # Build Server
    echo "Building Server on Ubuntu..."
    mkdir -p "$PROJECT_ROOT/Server/build"
    cd "$PROJECT_ROOT/Server/build"

    cmake "$PROJECT_ROOT/Server" -DDEBUG_BUILD=0
    make -j"$NUM_CORES"
    cd "$PROJECT_ROOT"

else
    error "Unsupported OS: $OS"
fi

echo "Build completed successfully."
