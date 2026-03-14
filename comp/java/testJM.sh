# examples:
# ./test.sh -l meta1
# ./test.sh -t meta2
for i in $2/*.java; do
    echo "=== $i ==="
    #comprobamos si continene _e1 en ese caso lo lanzamos sin -l, solo sirve para meta1
    if [[ "$i" == *_e1* ]]; then
        ./jucompiler < "$i" | diff "${i/%.java}.out" -
    #de forma similar si contiene _e2 lo lanzamos sin -t, solo sirve para meta2
    elif [[ "$i" == *_e2* ]]; then
        ./jucompiler < "$i" | diff "${i/%.java}.out" -
    else
        ./jucompiler $1 < "$i" | diff "${i/%.java}.out" -
    fi
    if [ ${PIPESTATUS[1]} -ne 0 ]; then
        echo "FAIL: $i"
    fi
done