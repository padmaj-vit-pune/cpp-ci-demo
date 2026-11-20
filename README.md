# C++ CI Pipeline Demo

Demonstrates a Continuous Integration workflow using GitHub Actions, `g++`, a shell test script, and `cppcheck`.

## Local Build, Test, and Lint

```bash
g++ -std=c++17 -Wall -Wextra -Werror src/calc.cpp -o calc
./tests/run_tests.sh
cppcheck --enable=warning,style,performance,portability --error-exitcode=1 src/
