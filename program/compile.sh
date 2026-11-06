#!/bin/bash

# Ensure emsdk is loaded
source ~/emsdk/emsdk_env.sh

echo "Compiling..."

# Properly find all .cpp files in current and subdirectories
# This is more robust than the previous find command
SOURCES=$(find . -maxdepth 2 -name "*.cpp")

# Compile with explicit flags
em++ $SOURCES -I. -sFETCH -o index.html \
  -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 \
  -s SDL2_IMAGE_FORMATS='["png"]' \
  --preload-file images \
  --preload-file fonts \
  --use-preload-plugins \
  -s NO_EXIT_RUNTIME=1 \
  --shell-file shell.html

if [ $? -ne 0 ]; then
    echo "[!] COMPILATION FAILED!"
else
    echo "[OK] Done!"
    # Verify index.data size
    ls -lh index.data
fi

read -p "Press Enter to exit..."
