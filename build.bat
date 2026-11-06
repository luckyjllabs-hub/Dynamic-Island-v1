@echo off
setlocal
cd /d "%~dp0"

if not exist "bin" mkdir "bin"

echo Building Island for Windows...
g++ -std=c++20 -O3 -mwindows -municode ^
    src\island.cpp ^
    -o bin\Island.exe ^
    -static-libgcc -static-libstdc++ ^
    -lgdiplus -ldwmapi -lole32 -luuid -lshell32

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: bin\Island.exe
endlocal

