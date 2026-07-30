# Builds and runs the base64 parity test against the compiled core library.
#
# The fixtures it checks against come from PHP itself; regenerate them with
#   php tools\gen_base64_fixtures.php > tests\Base64Fixtures.inc
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\run_base64_test.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$lib = Resolve-CoreLib -RepoRoot (Get-Location).Path

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path 'build\test-base64' | Out-Null

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           # /MD matches the runtime CMake builds the library with; mixing them is a link error.
           "cl /nologo /std:c++20 /EHsc /W3 /bigobj /utf-8 /O2 /MD /Iinclude /Itests " +
           "/Fobuild\test-base64\ /Febuild\test-base64\Base64ParityTest.exe " +
           "tests\Base64ParityTest.cpp $lib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

& 'build\test-base64\Base64ParityTest.exe'
exit $LASTEXITCODE
