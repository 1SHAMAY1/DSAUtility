@echo off
setlocal enabledelayedexpansion

:: Set root and build dir
set "ROOT_DIR=%~dp0"
set "ROOT_DIR=%ROOT_DIR:~0,-1%"
set "BUILD_DIR=%ROOT_DIR%\build"

:: Delete old build directory
if exist "%BUILD_DIR%" (
    echo Removing old build directory...
    rmdir /s /q "%BUILD_DIR%"
)

:: Recreate build directory
mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

:: Run CMake generation
cmake "%ROOT_DIR%" || goto :build_failed

:: Build the project
cmake --build . || goto :build_failed

:: Check for Debug or flat demo.exe
set "DEMO_EXE_PATH=%BUILD_DIR%\Debug\demo.exe"
if not exist "!DEMO_EXE_PATH!" (
    set "DEMO_EXE_PATH=%BUILD_DIR%\demo.exe"
)

:: Run demo.exe if found
if exist "!DEMO_EXE_PATH!" (
    echo.
    echo Running demo.exe...
    "!DEMO_EXE_PATH!"
) else (
    echo.
    echo demo.exe not found!
    goto :build_failed
)

goto :end

:build_failed
echo.
echo ===== Build Failed or Incomplete =====
cd /d "%ROOT_DIR%"
pause
exit /b 1

:end
cd /d "%ROOT_DIR%"
endlocal
pause
