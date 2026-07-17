@echo off
setlocal enabledelayedexpansion
set EMSDK_QUIET=1

echo Compiling...

set SOURCES=
for /r UI %%f in (*.cpp) do set SOURCES=!SOURCES! "%%f"
for /r ecuations %%f in (*.cpp) do set SOURCES=!SOURCES! "%%f"
set SOURCES=!SOURCES! main.cpp

em++ %SOURCES% -IUI -sFETCH -o index.html -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS=["png"] --preload-file images --preload-file fonts -s NO_EXIT_RUNTIME=1 --shell-file shell.html

if %errorlevel% neq 0 (
    echo [!] COMPILATION FAILED!
) else (
    echo [OK] Done!
    if exist errors.txt del errors.txt
)
pause