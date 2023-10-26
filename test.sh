#!/bin/bash
make splc
for i in {1..13}; do
    i=$(printf "%02d" $i)
    echo "$i"
    bin/splc < test/phase1/basic/test_1_r$i.spl > test/phase1/basic/my_test_1_r$i.out
    diff test/phase1/basic/my_test_1_r$i.out test/phase1/basic/test_1_r$i.out > test/phase1/basic/diff_test_1_r$i

    if [ ! -s test/phase1/diff_test_1_r$i ]; then
        rm test/phase1/diff_test_1_r$i
    fi

done
