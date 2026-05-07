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
    
   if [ -z "$FLAG" ]; then
            # Sin flag (Meta 4): Compilamos y guardamos la salida inicial
            ./jucompiler < "$i" > tmp_comp.out
            
            # Comprobamos si la salida generó LLVM IR (buscando la declaración de printf)
            if grep -q "declare i32 @printf" tmp_comp.out; then
                # Es código válido, lo renombramos a .ll y lo ejecutamos
                mv tmp_comp.out tmp.ll
                
                if [ -f "${i/.java/.in}" ]; then
                    lli tmp.ll < "${i/.java/.in}" > tmp.out

                else
                    lli tmp.ll > tmp.out

                fi
                rm -f tmp.ll
            else
                # Hubo errores semánticos, no hay LLVM. La salida son los errores.
                mv tmp_comp.out tmp.out
            fi
            
            # Comparamos (ya sea el resultado de lli o los errores impresos)
            diff "${i/.java/.out}" tmp.out
            DIFF_STATUS=$?
            
            rm -f tmp.out tmp_comp.out
            
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