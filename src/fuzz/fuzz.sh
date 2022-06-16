#!/bin/bash
set -eoxu pipefail

cd "$(dirname "$0")"

make -C .. FUZZ=1
afl-fuzz -i seeds/ -o output/ ../build-fuzz/bin/assemble @@ out
