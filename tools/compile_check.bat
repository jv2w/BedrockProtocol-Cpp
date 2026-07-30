@echo off
REM Compiles the given translation units (or every core .cpp when no argument is passed) without
REM linking, so that a porting batch can be syntax-checked before the full CMake build exists.
REM Usage: compile_check.bat [file1.cpp file2.cpp ...]
setlocal
cd /d "%~dp0\.."

for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set VSPATH=%%i
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

if not exist build\objcheck mkdir build\objcheck

if "%~1"=="" (
    set SOURCES=
    for /r src %%f in (*.cpp) do call set SOURCES=%%SOURCES%% "%%f"
) else (
    set SOURCES=%*
)

cl /nologo /c /std:c++20 /EHsc /W3 /bigobj /permissive- /utf-8 /Iinclude /Fobuild\objcheck\ %SOURCES%
exit /b %errorlevel%
