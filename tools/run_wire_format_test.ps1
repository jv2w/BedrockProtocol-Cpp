# Builds and runs the 2168 wire-format byte assertions against the compiled core library.
#
# This gate exists because the round-trip suites are blind to a mistake made consistently on both
# sides, and because the PHP wire-parity gate can no longer speak for 1.26.40 - the PHP original is
# archived at an older protocol. The expectations are written out by hand from gophertunnel v1.58.0.
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\run_wire_format_test.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$lib = Resolve-CoreLib -RepoRoot (Get-Location).Path

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path 'build\test-wire-format' | Out-Null

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           # /MD matches the runtime CMake builds the library with; mixing them is a link error.
           "cl /nologo /std:c++20 /EHsc /W3 /bigobj /utf-8 /O2 /MD /Iinclude /Itests " +
           "/Fobuild\test-wire-format\ /Febuild\test-wire-format\WireFormat2168Test.exe " +
           "tests\WireFormat2168Test.cpp $lib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

& 'build\test-wire-format\WireFormat2168Test.exe'
exit $LASTEXITCODE
