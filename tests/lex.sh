#!/usr/bin/env bash
set -exou pipefail

cd "$(dirname "$0")"
make -C ../src

for i in ../arm11_testsuite/test_cases/*.s
do
    echo "Testing $i"
    ../src/build/bin/assemble $i > /dev/null
done