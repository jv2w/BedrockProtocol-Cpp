<?php
/*
 * Hands this port's wire output to the PHP original and checks that PHP reads it back unchanged.
 *
 * tools/check_php_parity.py compares the two sources and proves they are spelled the same way. This
 * proves they behave the same way, which is not the same claim: the repeated-key case was identical
 * code in both languages that produced different packets, and no source comparison could have seen
 * it. Here PHP is a genuinely independent decoder, so a field this port writes at the wrong width,
 * in the wrong order, or with the wrong meaning comes back as different bytes.
 *
 * Run through tools/run_php_wire_parity.ps1, which builds the fixtures first.
 *
 *   php tools/php_wire_parity.php <path-to-pocketmine-source> <fixtures-file>
 */

declare(strict_types=1);

use pmmp\encoding\ByteBufferReader;
use pmmp\encoding\ByteBufferWriter;
use pocketmine\network\mcpe\protocol\PacketPool;

[$self, $phpRoot, $fixtureFile] = array_pad($argv, 3, null);
if ($phpRoot === null || $fixtureFile === null) {
    fwrite(STDERR, "usage: php php_wire_parity.php <pocketmine-source> <fixtures>\n");
    exit(2);
}
require rtrim($phpRoot, '\\/') . '/vendor/autoload.php';

/**
 * Divergences examined by hand and found to be the PHP original's own behaviour rather than a
 * defect in this port. Each one re-encodes to bytes PHP cannot itself reproduce from what it read,
 * so demanding equality here would be demanding the port reproduce a round-trip PHP fails.
 */
const ACCEPTED = [
    // BitSet::write shifts a signed 64-bit int, so once bit 63 of a part is set the sign extension
    // fills the low bits of the next byte and swallows the following part. PHP cannot read back what
    // it writes here - src/protocol/serializer/BitSet.cpp has the reproduction.
    'ClientMovementPredictionSyncPacket' => 'BitSet sign extension at bit 63',
    // EducationSettingsExternalLinkSettings::read passes the two strings to the constructor in the
    // opposite order to the one it read them in, so PHP transposes them on every decode.
    'EducationSettingsPacket' => 'external link url/displayName transposed by the original',
];

$pool = PacketPool::getInstance();
$total = $decodeFailed = $trailing = $mismatched = $unknown = $accepted = 0;
$reports = [];

foreach (file($fixtureFile, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES) as $line) {
    // The dump is redirected by PowerShell, which writes CRLF; FILE_IGNORE_NEW_LINES drops only the
    // LF, and a stray CR would make the hex an odd number of characters.
    $line = trim($line, " \t\r\n\xEF\xBB\xBF");
    if ($line === '' || $line[0] === '#') {
        continue;
    }
    [$name, $pid, $hex] = explode(' ', $line);
    if (strlen($hex) % 2 !== 0 || !ctype_xdigit($hex)) {
        fwrite(STDERR, "malformed fixture line for $name - regenerate the dump\n");
        exit(2);
    }
    $bytes = hex2bin($hex);
    $total++;

    $packet = $pool->getPacket($bytes);
    if ($packet === null) {
        $unknown++;
        $reports[] = "$name: PHP's PacketPool does not know packet id $pid";
        continue;
    }

    try {
        $in = new ByteBufferReader($bytes);
        $packet->decode($in);
    } catch (\Throwable $e) {
        $decodeFailed++;
        $reports[] = "$name: PHP could not decode this port's bytes: " . $e->getMessage();
        continue;
    }

    if ($in->getOffset() !== strlen($bytes)) {
        $trailing++;
        $reports[] = sprintf(
            "%s: PHP stopped after %d of %d bytes - this port wrote a field PHP does not read",
            $name,
            $in->getOffset(),
            strlen($bytes)
        );
        continue;
    }

    $out = new ByteBufferWriter();
    try {
        $packet->encode($out);
    } catch (\Throwable $e) {
        $mismatched++;
        $reports[] = "$name: PHP could not re-encode what it read: " . $e->getMessage();
        continue;
    }

    $again = $out->getData();
    if ($again === $bytes) {
        continue;
    }
    if (array_key_exists($name, ACCEPTED)) {
        $accepted++;
        continue;
    }

    // Locate the first differing byte, which is where the two implementations disagree.
    $limit = min(strlen($bytes), strlen($again));
    $at = $limit;
    for ($i = 0; $i < $limit; $i++) {
        if ($bytes[$i] !== $again[$i]) {
            $at = $i;
            break;
        }
    }
    $window = static fn(string $s): string => bin2hex(substr($s, max(0, $at - 8), 24));

    $mismatched++;
    $reports[] = sprintf(
        "%s: PHP re-encoded differently at offset %d (%d bytes vs %d)\n    cpp %s\n    php %s",
        $name,
        $at,
        strlen($bytes),
        strlen($again),
        $window($bytes),
        $window($again)
    );
}

foreach ($reports as $report) {
    echo $report . "\n";
}

printf("\n%d packets round-tripped through the PHP original\n", $total);
if ($accepted !== 0) {
    printf("  %d accepted divergence(s) skipped\n", $accepted);
}
$failed = $decodeFailed + $trailing + $mismatched + $unknown;
if ($failed === 0) {
    echo "[OK] PHP reproduced every packet byte for byte\n";
    exit(0);
}
printf(
    "[FAIL] %d packet(s): %d undecodable, %d left trailing bytes, %d re-encoded differently, %d unknown to PHP\n",
    $failed,
    $decodeFailed,
    $trailing,
    $mismatched,
    $unknown
);
exit(1);
