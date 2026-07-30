# Builds and runs the packet round-trip test against the compiled core library.
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\run_roundtrip_test.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$lib = Resolve-CoreLib -RepoRoot (Get-Location).Path

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path 'build\test' | Out-Null

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           # /MD matches the runtime CMake builds the library with; mixing them is a link error.
           # /EHa is required for _set_se_translator to turn hardware faults into C++ exceptions.
           "cl /nologo /std:c++20 /EHa /W3 /bigobj /utf-8 /O2 /MD /Iinclude /Itests " +
           "/Fobuild\test\ /Febuild\test\PacketRoundTripTest.exe " +
           "tests\PacketRoundTripTest.cpp $lib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

& 'build\test\PacketRoundTripTest.exe'
exit $LASTEXITCODE
