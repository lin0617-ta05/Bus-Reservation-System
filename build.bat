@echo off
setlocal
echo =======================================================================
echo Compiling Bus Reservation System (C Project - MCA Group 2)
echo =======================================================================

set PATH=C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;%PATH%

where gcc >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] GCC compiler was not found. Please install GCC or MSYS2.
    pause
    exit /b 1
)

echo Using compiler:
gcc --version | findstr /I "gcc"

echo.
echo Compiling source files: main.c bus.c passenger.c reservation.c utils.c ...
gcc -Wall -Wextra -std=c99 main.c bus.c passenger.c reservation.c utils.c -o bus_reservation.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo =======================================================================
    echo [SUCCESS] Compilation completed successfully!
    echo Executable generated: bus_reservation.exe
    echo =======================================================================
    echo Run the application by executing:
    echo   bus_reservation.exe
    echo =======================================================================
) else (
    echo.
    echo [ERROR] Compilation failed!
)
endlocal
