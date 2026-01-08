@echo off
set "ROOT_DIR=%~dp0"
set "PATH=%ROOT_DIR%w64devkit\bin;%PATH%"

echo Compiling Frames Modern version...
taskkill /F /IM frames-modern.exe 2>nul
gcc -Wall -O3 "%ROOT_DIR%main.c" "%ROOT_DIR%logic.c" -o "%ROOT_DIR%frames-modern.exe" -lgdi32 -luser32 -lcomctl32 -lcomdlg32 -mwindows -Wl,--stack,67108864

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  Compilation SUCCESSFUL!
    echo  Created: frames-modern.exe
    echo ========================================
    echo Launching app...
    start "" "%ROOT_DIR%frames-modern.exe"
) else (
    echo.
    echo ========================================
    echo  Compilation FAILED.
    echo ========================================
    pause
)
