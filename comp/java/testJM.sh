# examples:
# ./test.sh -l meta1
# ./test.sh -t meta2
for i in $2/*.java; do
    echo "=== $i ==="
    
    if [[ "$i" == *_e1* ]]; then
        ./jucompiler -e1 < "$i" | diff -u --color "${i/.java/.out}" -
    
    elif [[ "$i" == *_e2* ]]; then
        ./jucompiler -e2 < "$i" | diff -u --color "${i/_e2.java/.out}" -
    
    # caso por defecto
    else
        ./jucompiler $1 < "$i" | diff -u --color "${i/%.java}.out" -
    fi
    
    if [ ${PIPESTATUS[1]} -ne 0 ]; then
        echo "--> FAIL: Las salidas no coinciden en $i"
    fi
done