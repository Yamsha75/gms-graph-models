#/bin/bash

if [ $# -lt 1 ]; then
    echo "No test case supplied"
    exit 1
fi

TEMP=".tmp"

for TEST in "$@"; do
    FILE="tests/${TEST}.in"

    echo -n > $TEMP

    if [ -f $FILE ]; then
        while read LINE; do
            time ./gms $LINE >> $TEMP
        done < $FILE
    else
        echo "Test $TEST not found!"
    fi

    diff "tests/${TEST}.out" $TEMP
done

rm $TEMP

exit 0
