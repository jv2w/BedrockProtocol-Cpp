# Builds and runs the encoding test against the compiled core library.
#
# Replaces run_encoding_test.bat, which compiled only the test and Uuid.cpp. That was enough while
# the test covered the encoding primitives alone; it now also checks CommonTypes, so it links the
# library the same way every other gate does.
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\run_encoding_test.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$lib = Resolve-CoreLib -RepoRoot (Get-Location).Path

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path 'build\test-encoding' | Out-Null

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           # /MD matches the runtime CMake builds the library with; mixing them is a link error.
           "cl /nologo /std:c++20 /EHsc /W3 /bigobj /utf-8 /O2 /MD /Iinclude /Itests " +
           "/Fobuild\test-encoding\ /Febuild\test-encoding\EncodingTest.exe " +
           "tests\EncodingTest.cpp $lib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

& 'build\test-encoding\EncodingTest.exe'
exit $LASTEXITCODE
