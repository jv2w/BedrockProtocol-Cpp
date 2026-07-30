@echo off
REM Builds and runs the encoding test without needing the full CMake project, which cannot configure
REM until every type is ported.
setlocal
cd /d "%~dp0\.."

for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set VSPATH=%%i
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

if not exist build\test mkdir build\test

cl /nologo /std:c++20 /EHsc /W3 /utf-8 /Iinclude /Itests ^
   /Fobuild\test\ /Febuild\test\EncodingTest.exe ^
   tests\EncodingTest.cpp src\uuid\Uuid.cpp
if errorlevel 1 exit /b 1

build\test\EncodingTest.exe
exit /b %errorlevel%
