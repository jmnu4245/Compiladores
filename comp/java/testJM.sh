# examples:
# ./test.sh -l meta1
# ./test.sh -t meta2
./build.sh
for i in $2/*.java; do
    echo "=== $i ==="
    
    if [[ "$i" == *_e1* ]]; then
        ./jucompiler -e1 < "$i" | diff  "${i/.java/.out}" -
    
    elif [[ "$i" == *_e2* ]]; then
        ./jucompiler -e2 < "$i" | diff "${i/.java/.out}" -
    
    # caso por defecto
    else
        ./jucompiler $1 < "$i" | diff "${i/%.java}.out" -
    fi
    
    if [ ${PIPESTATUS[1]} -ne 0 ]; then
        echo "--> FAIL: Las salidas no coinciden en $i"
    fi
done