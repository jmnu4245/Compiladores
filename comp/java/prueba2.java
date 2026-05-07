/* weakness_test.java
 * Un único archivo que ejercita los 11 puntos débiles de codegen.c.
 * Ejecutar sin argumentos para la salida estándar;
 * ejecutar con un argumento entero (ej: 42) para ejercitar W9.
 *
 * Mapa de debilidades:
 *   W1  – mangling call-site vs. declaración (int→double)
 *   W2  – dos clases con el mismo nombre de método → redefinición
 *   W3  – dos clases con main → redefinición de @main
 *   W4  – String[] .length en función no-main
 *   W5  – variable local que oculta campo global
 *   W6  – if consume tmp_counter antes de evaluar la condición compleja
 *   W7  – literal decimal largo → desbordamiento de clean[64]
 *   W8  – string con chars no-imprimibles → desbordamiento strlit_llvm_content[1024]
 *   W9  – ParseArgs: +1 en índice (args[0] accede a argv[1])
 *   W10 – varios string literals distintos en el mismo programa
 *   W11 – return explícito en main con current_ret_type incorrecto
 */

/* ─────────────────────────────────────────────────────────────────────────
 * Clase auxiliar
 *   W2: tiene un método compute(int) con el mismo nombre que Main.compute.
 *       Sin prefijo de clase en el mangling ambas se llaman @_compute_i.
 *   W3: tiene su propio main, que también intentará emitirse como @main.
 * ───────────────────────────────────────────────────────────────────────── */
class Helper {

    /* W2 – mismo nombre y firma que Main.compute */
    public static int compute(int n) {
        return n * 3;
    }

    /* W4 – parámetro String[] en función no-main: a.length → %__a_len
     *       que nunca se allocatea en el bloque else del codegen */
    public static int countArgs(String[] a) {
        return a.length;
    }

    /* W3 – segundo main → redefinición de @main en LLVM */
    public static void main(String[] args) {
        System.out.print(999);
    }
}

/* ─────────────────────────────────────────────────────────────────────────
 * Clase principal
 * ───────────────────────────────────────────────────────────────────────── */
