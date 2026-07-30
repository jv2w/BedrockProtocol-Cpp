# Compares this port's wire output against the PHP original, by running both.
#
# check_php_parity.py compares the two sources; this compares what they actually produce. It dumps
# every selected packet fully populated, then hands the bytes to PHP to decode and re-encode. PHP
# returning identical bytes means it read every field the way this port wrote it.
#
# Needs a PHP with ext-encoding, which PocketMine ships in its own bin/php. Point -Php at it and
# -PhpRoot at a composer install of pocketmine/bedrock-protocol at the same protocol version.
#
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\run_php_wire_parity.ps1 `
#       -Php <...>\bin\php\php.exe -PhpRoot <...>\source

param(
    [Parameter(Mandatory = $true)][string]$Php,
    [Parameter(Mandatory = $true)][string]$PhpRoot,
    [string]$OutDir = 'build\wire-parity'
)

$ErrorActionPreference = 'Stop'
Set-Location (Join-Path $PSScriptRoot '..')

. (Join-Path $PSScriptRoot 'CoreLib.ps1')
$coreLib = Resolve-CoreLib -RepoRoot (Get-Location).Path

if (-not (Test-Path $Php)) { throw "php not found at $Php" }
if (-not (Test-Path (Join-Path $PhpRoot 'vendor\autoload.php'))) {
    throw "no vendor\autoload.php under $PhpRoot - point -PhpRoot at a composer install"
}

$verifySources = @(Get-ChildItem -Path 'verify\src' -Filter *.cpp -Recurse | ForEach-Object { $_.FullName })
if ($verifySources.Count -eq 0) { throw "no sources found under verify\src." }

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$responseFile = Join-Path $OutDir 'sources.rsp'
$verifySources | Set-Content -Path $responseFile -Encoding ascii

$exePath = Join-Path $OutDir 'DumpWireFixtures.exe'
$command = "call `"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul 2>&1 && " +
           "set VSLANG=1033 && " +
           "cl /nologo /std:c++20 /EHsc /W3 /bigobj /utf-8 /O2 /MD /MP " +
           "/Iinclude /Iverify/include /Itests " +
           "/Fo$OutDir\ /Fe$exePath tests\DumpWireFixtures.cpp `@$responseFile $coreLib"
cmd /c $command
if ($LASTEXITCODE -ne 0) { throw "compile failed" }

$fixtures = Join-Path $OutDir 'wire.txt'
# Not `>`: that writes a UTF-8 BOM here, which PHP would read as part of the first line.
& $exePath | Out-File -FilePath $fixtures -Encoding ascii
if ($LASTEXITCODE -ne 0) { throw "some packets could not be dumped; see the messages above" }

& $Php (Join-Path $PSScriptRoot 'php_wire_parity.php') $PhpRoot $fixtures
exit $LASTEXITCODE
