#!/bin/bash

./build.sh

if [ "$#" -eq 1 ]; then
    FLAG=""
    DIR="$1"
else
    FLAG="$1"
    DIR="$2"
fi

for i in "$DIR"/*.java; do
    echo "=== $i ==="
    
    CURRENT_FLAG="$FLAG"
    
    if [[ "$i" == *_e1* ]]; then
        CURRENT_FLAG="-e1"
    elif [[ "$i" == *_e2* ]]; then
        CURRENT_FLAG="-e2"
    fi
    
    ./jucompiler $CURRENT_FLAG < "$i" | diff "${i/.java/.out}" -
done