AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -M fuzz-main ../build-fuzz/bin/assemble @@ out-main-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub1 ../build-fuzz/bin/assemble @@ out-sub1-bin



AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub5 ../build-fuzz/bin/assemble @@ out-sub5-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub6 ../build-fuzz/bin/assemble @@ out-sub6-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub7 ../build-fuzz/bin/assemble @@ out-sub7-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub8 ../build-fuzz/bin/assemble @@ out-sub8-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub9 ../build-fuzz/bin/assemble @@ out-sub9-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub10 ../build-fuzz/bin/assemble @@ out-sub10-bin
AFL_SKIP_CPUFREQ=1 afl-fuzz -i seeds/ -o out-main -S fuzz-sub11 ../build-fuzz/bin/assemble @@ out-sub11-bin
n




wsl
ssh ilab
cd /data/ne321/arm11_39/src/fuzz/
