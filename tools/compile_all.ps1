# Compiles every core translation unit (no linking, no bridge) and writes the full log to
# build\compile_all.log. Use this to check the whole library without waiting for a CMake configure.
#
# Usage:  powershell -ExecutionPolicy Bypass -File tools\compile_all.ps1

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath

$buildDir = 'build'
New-Item -ItemType Directory -Force -Path "$buildDir\objall" | Out-Null

# Enumerate real files: many type directories are header-only, and an empty wildcard is a hard
# error for cl.
$sources = Get-ChildItem -Recurse -Path src -Filter *.cpp |
    Where-Object { $_.FullName -notmatch '\\src\\bridge\\' } |
    ForEach-Object { '"' + $_.FullName + '"' }

Write-Output "Compiling $($sources.Count) translation units..."

# cl's command line would overflow, so pass the file list in a response file.
$responseFile = "$buildDir\compile_all.rsp"
Set-Content -Path $responseFile -Value ($sources -join "`n") -Encoding ascii

$logFile = "$buildDir\compile_all.log"
$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           "cl /nologo /c /std:c++20 /EHsc /W3 /bigobj /permissive- /utf-8 /MP /Iinclude " +
           "/Fobuild\objall\ @$responseFile > $logFile 2>&1"
cmd /c $command
$exitCode = $LASTEXITCODE

$errors = Select-String -Path $logFile -Pattern ': (fatal )?error' -AllMatches
Write-Output "Exit code $exitCode. Errors: $($errors.Count). Full log: $logFile"
exit $exitCode
