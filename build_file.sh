#!/bin/bash

# --- Configuration Variables ---
BUILD_DIR="build"
SRC_DIR="image_lib"
TESTS_DIR="tests"
BIN_DIR="bin"

# Common GCC flags (warnings, debug info, include path)
# CFLAGS="-Wall -Wextra -g -I$INCLUDE_DIR"

# Linker Flags:
# Use pkg-config to get necessary SDL2 flags (cflags and libs)

# Add the math library linker flag
# Get OpenEXR flags. pkg-config is the safest way on Fedora.
# 1. Capture the flags (using $() is correct)
EXR_CFLAGS=$(pkg-config --cflags OpenEXR Imath)
EXR_LIBS=$(pkg-config --libs OpenEXR Imath)

# DEBUG: Remove this once it works. It will show you if the paths are empty.
echo "Debug: EXR_CFLAGS is [$EXR_CFLAGS]"

MATH_FLAG="-lm -lbsd"
PTHREAD_FLAG="-pthread"
# Combine your existing flags with the new EXR libraries
LINK_FLAGS="$MATH_FLAG $EXR_LIBS $PTHREAD_FLAG"

# Ensure the build directory exists
mkdir -p $BUILD_DIR $BIN_DIR

# --- Build Functions ---

build_main() {
  APP_NAME="exec"
  echo "--- Building Main Application: $APP_NAME ---"

  # Compile and link
  gcc $CFLAGS -I$SRC_DIR $EXR_CFLAGS "$SRC_DIR/main.c" -o "$BUILD_DIR/$APP_NAME" $LINK_FLAGS
  # gcc $CFLAGS "$SRC_DIR/main.c" -o $BUILD_DIR/$APP_NAME $LINK_FLAGS

  if [ $? -eq 0 ]; then
    mv $BUILD_DIR/$APP_NAME $BIN_DIR/$APP_NAME
    echo "Main application build successful! Executable is in ./bin/$APP_NAME"
  else
    echo "Main application build failed!"
    exit 1
  fi
}

# --- Command Line Argument Handling ---

case "$1:$2:$3" in
main:"":"")
  CFLAGS="-g -std=gnu99 "
  build_main
  ;;
*)
  echo "Invalid Build Arguments! Valid Arguments Are:"
  echo "./build.sh main  (Builds the main application)"
  echo "./build.sh tests (Builds the testing application)"
  echo "./build.sh main warn  (Builds the main application with stricter warnings)"
  echo "./build.sh tests warn (Builds the testing application stricter warnings)"
  echo "./build.sh main fast(Builds the main application with high optimisation)"
  echo "./build.sh main fast warn  (Builds the main application with stricter warnings and high optimisation)"
  exit 1
  ;;
esac
