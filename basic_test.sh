#!/bin/bash

rm -f std-test/diff* std-test/my*

for i in {1..12}; do
    i=$(printf "%02d" $i)
    bin/spl_parser std-test/test_1_r$i.spl >> std-test/my_test_1_r$i.out 2>&1
    diff std-test/my_test_1_r$i.out std-test/test_1_r$i.out > std-test/diff_test_1_r$i

    if [ ! -s std-test/diff_test_1_r$i ]; then
        echo "$i" passed
        rm std-test/diff_test_1_r$i
    else
        echo "$i" failed
    fi
done