#!/bin/bash

./build.sh

for path in meta4/*.java; do
    b="${path%.java}"
    f=$(basename "$path")
    
    echo "=== $f ==="
    
    ./jucompiler < "$path" > tmp.ll
    
    ARGS=""
    case "$f" in
        "MultipleParametersCommandLine.java") ARGS="1 2" ;;
        "strings_tests.java") ARGS="1 2 3 4 5" ;;
        "length.java") ARGS="a b" ;;
        "fluxControl1.java") ARGS="15" ;;
        "fluxControl2.java") ARGS="30" ;;
        "fluxControl3.java") ARGS="30" ;;
        "randomTest.java") ARGS="7" ;;
    esac

    if [ -f "$b.in" ]; then
        lli tmp.ll $ARGS < "$b.in" > tmp.out 2>&1
    else
        lli tmp.ll $ARGS > tmp.out 2>&1
    fi
    
    if [ -f "$b.out" ]; then
        if diff -q "$b.out" tmp.out >/dev/null; then
            echo "✅ $f"
        else
            echo "❌ $f"
            diff "$b.out" tmp.out
        fi
    else
        echo "⚠️ $f (Sin .out)"
        cat tmp.out 
    fi
    
    rm -f tmp.ll tmp.out
done