# Builds and runs the packet round-trip test against the compiled core library.
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\run_roundtrip_test.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

# The library is built by the parent project's CMake tree, not by a build directory of our own.
# Ninja (single-config) puts it directly under the target directory; the old Visual Studio generator
# used a Release/ subdirectory, so both are accepted.
$lib = @('..\build\bedrock_protocol\bedrock_protocol.lib',
         '..\build\bedrock_protocol\Release\bedrock_protocol.lib') |
    Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $lib) {
    throw "bedrock_protocol.lib not found - build the parent project first (build.bat)."
}
$lib = (Resolve-Path $lib).Path

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
