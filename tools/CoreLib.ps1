# Locates bedrock_protocol.lib for the offline gates, building it if necessary.
#
# This repository used to live inside a plugin project, so the gates simply reached up into that
# project's CMake tree for the library. It stands alone now, and anyone who clones it must be able to
# run the gates without a consumer project, so the repository builds its own copy on demand.
#
# A sibling consumer's build tree is still preferred when present: it is the exact library the plugin
# ships, so the gate tests what actually runs rather than a second build of the same sources.

function Resolve-CoreLib {
    param([string]$RepoRoot)

    # Ninja (single-config) puts the library directly under the target directory; the Visual Studio
    # generator uses a Release/ subdirectory.
    $candidates = @()
    foreach ($tree in @(Get-ChildItem -Directory (Join-Path $RepoRoot '..') -ErrorAction SilentlyContinue)) {
        $candidates += Join-Path $tree.FullName 'build\bedrock_protocol\bedrock_protocol.lib'
        $candidates += Join-Path $tree.FullName 'build\bedrock_protocol\Release\bedrock_protocol.lib'
    }
    $candidates += Join-Path $RepoRoot 'build\bedrock_protocol.lib'
    $candidates += Join-Path $RepoRoot 'build\Release\bedrock_protocol.lib'

    $found = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if ($found) {
        return (Resolve-Path $found).Path
    }

    Write-Host "bedrock_protocol.lib not found - building it..."
    $build = Join-Path $RepoRoot 'build'
    & cmake -S $RepoRoot -B $build -DCMAKE_BUILD_TYPE=Release | Out-Null
    if ($LASTEXITCODE -ne 0) { throw "cmake configure failed." }
    & cmake --build $build --config Release --target bedrock_protocol | Out-Null
    if ($LASTEXITCODE -ne 0) { throw "cmake build failed." }

    $found = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $found) { throw "bedrock_protocol.lib still missing after building." }
    return (Resolve-Path $found).Path
}
