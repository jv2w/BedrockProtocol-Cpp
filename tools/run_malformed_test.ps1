# Builds and runs the malformed-input robustness suite against the compiled core library.
#
# Complement to run_deep_roundtrip.ps1. That script fuzzes shapes derived from VALID packets and only
# covers the packets that have fillers; this one feeds purely hostile buffers to EVERY registered
# packet ID and asserts only that the process survives and every failure is std::exception-derived -
# the exact contract bridge::PacketInterceptor::dispatch depends on.
#
# Like the deep suite it compiles with MSVC cl and /EHa on purpose: _set_se_translator is what turns
# a hardware fault into a reportable FAULTED result rather than a dead test process, and clang-cl
# accepts /EHa without implementing the translator.
#
# It links ONLY the prebuilt core library - no verify/ sources - so it shares no state with the
# filler-based suites and cannot race them.
#
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\run_malformed_test.ps1

param(
    # Separate output directory per caller so parallel audits never overwrite each other's objects.
    [string]$OutDir = 'build\test-fuzz'
)

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

# The core library is taken from the parent project's CMake tree, exactly as the deep suite does.
# This script never builds it (build.bat would race sibling audits).
$coreLib = @('..\build\bedrock_protocol\bedrock_protocol.lib',
             '..\build\bedrock_protocol\Release\bedrock_protocol.lib') |
    Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $coreLib) {
    throw "bedrock_protocol.lib not found - build the parent project first (build.bat)."
}
$coreLib = (Resolve-Path $coreLib).Path

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$exePath = Join-Path $OutDir 'MalformedInputTest.exe'

# The NBT sources are recompiled here and listed BEFORE the core library on the link line, so these
# objects satisfy the NBT symbols and the library's stale copies are never pulled in. Everything else
# still comes from the prebuilt .lib.
#
# This exists because this suite's findings are IN the NBT layer (see the ReaderTracker depth-unwind
# fix), and this script must not run build.bat - it would race sibling audits. Without the overlay the
# suite would still be testing the unfixed code and would die on the first nested-NBT case.
#
# It is an overlay, not a substitute: the fixes must still be rebuilt into the core library centrally.
$nbtSources = @(Get-ChildItem -Path 'src\nbt' -Filter *.cpp -Recurse | ForEach-Object { $_.FullName })
$nbtSources += (Resolve-Path 'src\protocol\serializer\NetworkNbtSerializer.cpp').Path
# Also overlaid because the fix that reaches it is the changed default argument in
# BaseNbtSerializer.h, which only takes effect in translation units recompiled against the header.
$nbtSources += (Resolve-Path 'src\protocol\LevelEventGenericPacket.cpp').Path
$responseFile = Join-Path $OutDir 'malformed_nbt.rsp'
$nbtSources | Set-Content -Path $responseFile -Encoding ascii

# /EHa is required for _set_se_translator to turn hardware faults into C++ exceptions.
# /MD matches the runtime CMake builds the core library with; mixing them is a link error.
$flags = "/nologo /std:c++20 /EHa /W3 /bigobj /utf-8 /O2 /MD"

$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           "cl $flags " +
           "/Iinclude /Itests " +
           "/Fo$OutDir\ /Fe$exePath " +
           "tests\MalformedInputTest.cpp `@$responseFile $coreLib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

& $exePath
exit $LASTEXITCODE
