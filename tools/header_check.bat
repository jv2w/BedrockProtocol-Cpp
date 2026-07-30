@echo off
REM Compiles every header under a directory in isolation, catching missing includes and bad
REM signatures in header-only types that no .cpp happens to pull in.
REM Usage: header_check.bat <directory relative to include/bedrock_protocol>
setlocal enabledelayedexpansion
cd /d "%~dp0\.."

for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set VSPATH=%%i
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

if not exist build\hdrcheck mkdir build\hdrcheck
del /q build\hdrcheck\*.cpp >nul 2>&1

set COUNT=0
for /r "include\bedrock_protocol\%~1" %%f in (*.h) do (
    set /a COUNT+=1
    set "REL=%%f"
    set "REL=!REL:%CD%\include\=!"
    set "REL=!REL:\=/!"
    echo #include "!REL!">> build\hdrcheck\all_headers.cpp
)
echo int main() { return 0; }>> build\hdrcheck\all_headers.cpp

echo Checking %COUNT% headers under %~1 ...
cl /nologo /c /std:c++20 /EHsc /W3 /bigobj /permissive- /utf-8 /Iinclude /Fobuild\hdrcheck\ build\hdrcheck\all_headers.cpp
set RESULT=%errorlevel%
del /q build\hdrcheck\all_headers.cpp >nul 2>&1
exit /b %RESULT%
