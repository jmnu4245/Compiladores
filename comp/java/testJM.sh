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
    
    if [[ "$i" == *_e1* ]]; then
        ./jucompiler -e1 < "$i" | diff "${i/.java/.out}" -
    
    elif [[ "$i" == *_e2* ]]; then
        ./jucompiler -e2 < "$i" | diff "${i/.java/.out}" -
    
    else
        if [ -z "$FLAG" ]; then
            # Sin flag (Meta 4): Generar LLVM IR, ejecutarlo y comparar su salida
            ./jucompiler < "$i" > tmp.ll
            
            # Si existe un archivo de entrada (.in) para el test, se lo pasamos por stdin a lli
            if [ -f "${i/.java/.in}" ]; then
                lli tmp.ll < "${i/.java/.in}" > tmp.out
            else
                lli tmp.ll > tmp.out
            fi
            
            diff "${i/.java/.out}" tmp.out
            DIFF_STATUS=$?
            
            rm -f tmp.ll tmp.out
            
            if [ $DIFF_STATUS -ne 0 ]; then
                echo "--> FAIL: Las salidas no coinciden en $i"
            fi
        else
            # Con flag (Meta 1, 2, 3): Comparar directamente la salida del compilador
            ./jucompiler $FLAG < "$i" | diff "${i/.java/.out}" -
            
            if [ ${PIPESTATUS[1]} -ne 0 ]; then
                echo "--> FAIL: Las salidas no coinciden en $i"
            fi
        fi
    fi
done