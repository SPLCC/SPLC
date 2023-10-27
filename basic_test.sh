#!/bin/bash

rm -f tests/phase1/basic/diff* tests/phase1/basic/my*

for i in {1..12}; do
    i=$(printf "%02d" $i)
    bin/spl_parser < tests/phase1/basic/test_1_r$i.spl >> tests/phase1/basic/my_test_1_r$i.out 2>&1
    diff tests/phase1/basic/my_test_1_r$i.out tests/phase1/basic/test_1_r$i.out > tests/phase1/basic/diff_test_1_r$i

    if [ ! -s tests/phase1/basic/diff_test_1_r$i ]; then
        echo "$i" passed
        rm tests/phase1/basic/diff_test_1_r$i
    else
        echo "$i" failed
    fi
done