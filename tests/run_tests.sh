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

assert_approx() {
    local actual="$1"
    local expected="$2"
    local tol="$3"
    local label="$4"

    awk -v a="$actual" -v e="$expected" -v t="$tol" -v lbl="$label" '
        BEGIN {
            diff = (a > e) ? (a - e) : (e - a);
            if (diff > t) {
                printf "FAIL [%s]: expected ~%s (±%s), got %s\n", lbl, e, t, a;
                exit 1;
            }
            printf "PASS [%s]: got %s (expected ~%s)\n", lbl, a, e;
        }
    '
}

assert_fails() {
    local label="$1"
    shift
    set +e
    "$@" >/dev/null 2>&1
    local code=$?
    set -e
    if [ "$code" -eq 0 ]; then
        echo "FAIL [$label]: expected non-zero exit code, got 0"
        exit 1
    fi
    echo "PASS [$label]: failed as expected with exit code $code"
}

assert_eq "$("$BIN" add 2 3)" "5" "add positive numbers"
assert_eq "$("$BIN" sub 10 4)" "6" "subtract numbers"
assert_eq "$("$BIN" mul 3 7)" "21" "multiply numbers"
assert_eq "$("$BIN" div 20 4)" "5" "divide numbers"
assert_eq "$("$BIN" pow 2 5)" "32" "power 2^5"
assert_eq "$("$BIN" sqrt 81)" "9" "sqrt of 81"
assert_approx "$("$BIN" sin_deg 30)" "0.5" "1e-6" "sin(30 deg)"
assert_approx "$("$BIN" cos_deg 60)" "0.5" "1e-6" "cos(60 deg)"
assert_approx "$("$BIN" log10 1000)" "3" "1e-6" "log10(1000)"

assert_fails "division by zero" "$BIN" div 5 0
assert_fails "sqrt of negative" "$BIN" sqrt -9
assert_fails "ln of zero" "$BIN" ln 0
assert_fails "missing arguments" "$BIN" add 1

echo "All scientific calculator tests passed."
