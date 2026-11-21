# C++ Scientific Calculator & CI Pipeline

Demonstrates a Continuous Integration workflow using GitHub Actions, `g++`, a shell test suite (`tests/run_tests.sh`), and `cppcheck`, paired with a local web UI (`web/index.html`) served by `web/server.py`.

## CI Pipeline Overview

The workflow in `.github/workflows/ci.yml` runs automatically on pushes to `main` and on Pull Requests targeting `main`:

1. **Automatic Compilation:** Compiles `src/calc.cpp` with `g++ -std=c++17 -Wall -Wextra -Werror` into `./calc`.
2. **Automated Testing:** Executes `./tests/run_tests.sh` to verify arithmetic, scientific functions, angle modes, and error handling (such as division by zero and invalid inputs).
3. **Pull Request Validation:** Runs the full compile, test, and static analysis checks on Pull Requests before changes are merged into `main`.
4. **Code Quality Checking:** Runs `cppcheck --enable=warning,style,performance,portability --error-exitcode=1 src/` to catch warnings, style issues, performance pitfalls, and portability problems.

## Local Build, Test, and Static Analysis (Fedora)

```bash
g++ -std=c++17 -Wall -Wextra -Werror src/calc.cpp -o calc
./tests/run_tests.sh
cppcheck --enable=warning,style,performance,portability --error-exitcode=1 src/
