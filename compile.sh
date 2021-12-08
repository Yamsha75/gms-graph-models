#/bin/bash

g++ *.cpp --output gms

EXIT_CODE=$?
if [[ $EXIT_CODE -eq 0 ]]; then
    echo "OK"
else
    echo "ERROR"
fi

exit $EXIT_CODE
