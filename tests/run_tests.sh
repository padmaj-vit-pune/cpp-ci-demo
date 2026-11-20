#!/usr/bin/env bash
set -euo pipefail

BIN="./calc"

if [ ! -x "$BIN" ]; then
    echo "Binary $BIN not found or not executable"
    exit 1
fi

assert_eq() {
    local actual="$1"
    local expected="$2"
    local label="$3"

    if [ "$actual" != "$expected" ]; then
        echo "FAIL [$label]: expected '$expected', got '$actual'"
        exit 1
    fi
    echo "PASS [$label]: got '$actual'"
}

assert_eq "$("$BIN" 2 3)" "5" "add positive numbers"
assert_eq "$("$BIN" -4 10)" "6" "add negative and positive"
assert_eq "$("$BIN" 0 0)" "0" "add zeros"

set +e
"$BIN" 1 >/dev/null 2>&1
EXIT_CODE=$?
set -e

assert_eq "$EXIT_CODE" "1" "missing arguments returns exit code 1"

echo "All tests passed."
