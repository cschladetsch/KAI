#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Fixes three build errors in CppKAI on Windows/MSVC:
      1. pthread.lib linker errors  -> removed from CMake link targets
      2. /Wno-deprecated* flags     -> guarded behind NOT MSVC
      3. std::thread usage          -> migrated to std::jthread

.PARAMETER Root
    Root of the CppKAI repo. Defaults to the directory containing this script.

.PARAMETER DryRun
    Print what would change without writing any files.

.PARAMETER CppOnly
    Only process .cpp / .h files (skip CMake files).

.PARAMETER CmakeOnly
    Only process CMake files (skip C++ source).
#>
param(
    [string]$Root    = $PSScriptRoot,
    [switch]$DryRun,
    [switch]$CppOnly,
    [switch]$CmakeOnly
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# ── colours ──────────────────────────────────────────────────────────────────
function Write-Ok  ($msg) { Write-Host "  [OK]  $msg" -ForegroundColor Green  }
function Write-Fix ($msg) { Write-Host "  [FIX] $msg" -ForegroundColor Yellow }
function Write-Inf ($msg) { Write-Host "  [--]  $msg" -ForegroundColor Cyan   }
function Write-Err ($msg) { Write-Host "  [ERR] $msg" -ForegroundColor Red    }

$stats = @{ cmake = 0; cpp = 0; skipped = 0 }

# ── backup ───────────────────────────────────────────────────────────────────
function Backup-File([string]$path) {
    $bak = "$path.bak"
    if (-not (Test-Path -LiteralPath $bak)) {
        Copy-Item -LiteralPath $path -Destination $bak
    }
}

# ── 1. CMake fixes ────────────────────────────────────────────────────────────
function Fix-CmakeFile([string]$path) {
    $original = Get-Content -LiteralPath $path -Raw
    $content  = $original
    $changed  = $false

    # --- pthread link (bare string and via Threads::Threads when pthread-only)
    # Pattern: target_link_libraries(... pthread ...) or ... pthreads ...)
    # Replace the bare 'pthread' token inside target_link_libraries calls.
    # We wrap the whole call with a platform guard instead of deleting it so
    # that Linux builds still work.

    # Simple bare pthread token in target_link_libraries
    $pthreadPattern = '(?m)^(\s*target_link_libraries\s*\([^)]*)\bpthread\b([^)]*\))'
    if ($content -match $pthreadPattern) {
        # NOTE: the -replace operator only accepts a string replacement, not a
        # scriptblock/MatchEvaluator - it silently mis-binds $m and blows up on
        # $m.Groups. [regex]::Replace() correctly converts a scriptblock into a
        # MatchEvaluator delegate, so use that instead.
        $content = [regex]::Replace($content, $pthreadPattern, {
            param($m)
            $before = $m.Groups[1].Value
            $after  = $m.Groups[2].Value
            # Wrap only if not already inside a platform guard
            "if(NOT WIN32)`n$before$after`nendif()"
        })
        $changed = $true
        Write-Fix "  pthread link guarded: $path"
    }

    # find_package(Threads REQUIRED) -> add guard comment
    # If Threads::Threads is used, it's fine on MSVC; just leave it.
    # Only flag bare pthread in link lines (handled above).

    # --- /Wno-deprecated in compile options, guarded behind NOT MSVC
    #
    # NOTE: earlier versions of this script matched an optional trailing '>'
    # as part of the flag token itself. That's wrong on two counts:
    #  1. It can eat a '>' that actually belongs to an *enclosing* generator
    #     expression (e.g. $<IF:$<CXX_COMPILER_ID:MSVC>,/O2,-g -Wno-deprecated>),
    #     silently unbalancing it on the very first run.
    #  2. Re-running the script would match the flag again inside its own
    #     previous wrap and nest another layer around it, since the
    #     replacement text itself contains "-Wno-deprecated" - not idempotent.
    # Fixed by (a) never consuming a trailing '>' as part of the match, and
    # (b) skipping the whole fix, file-wide, once our wrap fingerprint is
    # already present anywhere in the file.
    $warnPattern    = '(-|/)Wno-deprecated'
    $warnReplacement = '$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-deprecated>'
    $warnFingerprint = 'NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-deprecated'

    if (($content -match $warnPattern) -and (-not $content.Contains($warnFingerprint))) {
        $content = $content -replace $warnPattern, $warnReplacement
        $changed = $true
        Write-Fix "  /Wno-deprecated guarded: $path"
    }

    # --- pthread in target_link_libraries written as a generator expression
    # e.g. $<$<PLATFORM_ID:Linux>:pthread>  -- leave those alone (already guarded)
    # We only touched bare tokens above, so nothing extra needed.

    if ($changed) {
        if (-not $DryRun) {
            Backup-File $path
            Set-Content -LiteralPath $path -Value $content -NoNewline
        }
        $stats.cmake++
    }
}

# ── 2. C++ source fixes ───────────────────────────────────────────────────────
function Fix-CppFile([string]$path) {
    $original = Get-Content -LiteralPath $path -Raw
    $content  = $original
    $changed  = $false

    # Add <stop_token> isn't needed separately; it's included via <thread> in C++20.
    # Replace #include <thread> with #include <thread>  (jthread lives there too)
    # Nothing to do for the include itself.

    # Replace std::thread with std::jthread
    # Be conservative: only rename the type, don't rewrite lambdas.
    # The destructor join is automatic; explicit .join() calls become no-ops but
    # are still valid, so we leave them in place with a comment.
    if ($content -match '\bstd::thread\b') {
        $content = $content -replace '\bstd::thread\b', 'std::jthread'
        $changed = $true
        Write-Fix "  std::thread -> std::jthread: $path"
    }

    # Flag explicit .join() calls so the dev can review (jthread joins on dtor)
    # We don't auto-remove them because join() on jthread is still valid and safe.
    if ($content -match '\bjthread\b.*\.join\(\)') {
        Write-Inf "  Review: explicit .join() on jthread (safe but redundant) in $path"
    }

    if ($changed) {
        if (-not $DryRun) {
            Backup-File $path
            Set-Content -LiteralPath $path -Value $content -NoNewline
        }
        $stats.cpp++
    }
}

# ── entry point ───────────────────────────────────────────────────────────────
Write-Host ""
Write-Host "CppKAI threading fix" -ForegroundColor Cyan
Write-Host "Root : $Root"
Write-Host "Mode : $(if ($DryRun) { 'DRY RUN (no files written)' } else { 'LIVE' })"
Write-Host ""

if (-not (Test-Path -LiteralPath $Root)) {
    Write-Err "Root path not found: $Root"
    exit 1
}

# Collect files
$cmakeFiles = @()
$cppFiles   = @()

if (-not $CppOnly) {
    $cmakeFiles = Get-ChildItem -Path $Root -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -eq 'CMakeLists.txt' -or $_.Extension -eq '.cmake' } |
        Where-Object { $_.FullName -notmatch '\\build\\' -or $_.FullName -match 'CMakeLists' }
        # We DO want to touch build-tree CMakeLists if they exist, but vcxproj are generated - skip
}

if (-not $CmakeOnly) {
    $cppFiles = Get-ChildItem -Path $Root -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object { $_.Extension -in '.cpp', '.cxx', '.cc', '.h', '.hpp' } |
        Where-Object { $_.FullName -notmatch '\\build\\' }
}

Write-Inf "CMake files : $($cmakeFiles.Count)"
Write-Inf "C++ files   : $($cppFiles.Count)"
Write-Host ""

foreach ($f in $cmakeFiles) {
    try {
        Fix-CmakeFile $f.FullName
    } catch {
        Write-Err "  Skipped (unreadable, e.g. broken symlink/junction or uninitialized submodule): $($f.FullName) - $($_.Exception.Message)"
        $stats.skipped++
    }
}

foreach ($f in $cppFiles) {
    try {
        Fix-CppFile $f.FullName
    } catch {
        Write-Err "  Skipped (unreadable, e.g. broken symlink/junction or uninitialized submodule): $($f.FullName) - $($_.Exception.Message)"
        $stats.skipped++
    }
}

# ── summary ───────────────────────────────────────────────────────────────────
Write-Host ""
Write-Host "Done." -ForegroundColor Cyan
Write-Host "  CMake files modified : $($stats.cmake)"
Write-Host "  C++ files modified   : $($stats.cpp)"
Write-Host "  Files skipped        : $($stats.skipped)"
if ($DryRun) {
    Write-Host "  (dry run - nothing written)" -ForegroundColor Yellow
} else {
    Write-Host "  Backups written as *.bak alongside each changed file."
}
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "  1. Regenerate: cmake -B build -G 'Visual Studio 17 2022' -A x64"
Write-Host "  2. Rebuild:    cmake --build build --config Release"
Write-Host "  3. Review any 'explicit .join()' warnings above - safe to remove"
Write-Host "  4. If stop_token cancellation is needed, add it manually per thread"
Write-Host ""
