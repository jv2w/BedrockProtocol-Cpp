# Builds and runs the deep round-trip suite against the compiled core library.
#
# This is the AUTHORITATIVE gate. It compiles with MSVC cl rather than clang-cl on purpose: the
# truncation and corruption stages need _set_se_translator to tell "threw cleanly" apart from
# "faulted", and clang-cl accepts /EHa without implementing the translator. The in-game selftest runs
# the same engine but skips those stages and says so.
#
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\run_deep_roundtrip.ps1
#   powershell -ExecutionPolicy Bypass -File tools\run_deep_roundtrip.ps1 -Only MovePlayerPacket
#   powershell -ExecutionPolicy Bypass -File tools\run_deep_roundtrip.ps1 -Seed 0x1234 -NoFuzz

param(
    [string]$Only = '',
    [string]$Seed = '',
    [switch]$NoFuzz,
    # Separate output directory per caller. Parallel filler batches each verify their own work, and
    # sharing build\test would have them overwrite each other's object files mid-compile.
    [string]$OutDir = 'build\test',
    # Build with clang-cl instead of MSVC, reproducing the IN-GAME code path exactly.
    #
    # Worth running whenever the engine changes. The MSVC build catches a hardware fault and reports
    # it; the plugin build cannot, so a stage that provokes one is offline-green and fatal in-game -
    # which is precisely how the anti-tautology stage once took the live server down.
    [switch]$Clang
)

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$coreLib = Resolve-CoreLib -RepoRoot (Get-Location).Path

# The verify sources are compiled here from source rather than linked from
# bedrock_protocol_verify.lib, and that is the whole point of this script. CMake builds that library
# with clang-cl (Endstone forces it), and clang-cl accepts /EHa without implementing
# _set_se_translator - so a lib built there has the fuzz stages permanently disabled. Compiling them
# with MSVC here is what makes this the authoritative gate rather than a weaker copy of the in-game
# selftest.
$verifySources = @(Get-ChildItem -Path 'verify\src' -Filter *.cpp -Recurse |
    ForEach-Object { $_.FullName })
if ($verifySources.Count -eq 0) {
    throw "no sources found under verify\src."
}

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

# A response file keeps the command line under the 8191-character limit as the filler set grows.
$responseFile = Join-Path $OutDir 'deep_roundtrip.rsp'
$verifySources | Set-Content -Path $responseFile -Encoding ascii

$exePath = Join-Path $OutDir 'DeepRoundTripTest.exe'
if ($Clang) {
    # clang-cl accepts /EHa but does not implement _set_se_translator, so the engine detects that and
    # skips the stages that need it. /EHsc here keeps the EH model identical to the plugin build.
    $compiler = "$vsPath\VC\Tools\Llvm\x64\bin\clang-cl.exe"
    if (-not (Test-Path $compiler)) { throw "clang-cl not found at $compiler" }
    $flags = "/nologo /std:c++20 /EHsc /bigobj /utf-8 /O2 /MD"
} else {
    $compiler = 'cl'
    # /EHa is required for _set_se_translator to turn hardware faults into C++ exceptions.
    # /MD matches the runtime CMake builds the core library with; mixing them is a link error.
    $flags = "/nologo /std:c++20 /EHa /W3 /bigobj /utf-8 /O2 /MD /MP"
}

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           "`"$compiler`" $flags " +
           "/Iinclude /Iverify/include /Itests " +
           "/Fo$OutDir\ /Fe$exePath " +
           "tests\DeepRoundTripTest.cpp `@$responseFile $coreLib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

$testArgs = @()
if ($Only) { $testArgs += "--only=$Only" }
if ($Seed) { $testArgs += "--seed=$Seed" }
if ($NoFuzz) { $testArgs += '--no-fuzz' }

& $exePath @testArgs
exit $LASTEXITCODE
