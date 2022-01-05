#/bin/bash

if [ $# -eq 0 ]; then
    TESTS="0 1 2 3 4"
else
    TESTS="$*"
fi

TEMP=".tmp"

for TEST in $TESTS; do
    FILE="tests/${TEST}.in"

    if [ -f $FILE ]; then
        echo -n > $TEMP

        echo "Testing $TEST..."

        time ./gms < $FILE >> $TEMP

        diff "tests/${TEST}.out" $TEMP
    else
        echo "Test $TEST not found!"
    fi
done

rm $TEMP

exit 0
