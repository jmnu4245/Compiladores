class Main {

    /* W5 – campo global que una variable local puede ocultar (o no) */
    public static int globalX;

    /* W1 – parámetro double: llamar con literal int provoca mangling _i
     *       en el call-site pero _d en la definición → símbolo no encontrado */
    public static double twice(double x) {
        return x * 2.0;
    }

    /* W2 – mismo nombre que Helper.compute */
    public static int compute(int n) {
        return n + 1;
    }

    /* W5 – variable local globalX debe ocultar el campo estático globalX=10 */
    public static int shadowTest() {
        int globalX;
        globalX = 99;
        return globalX;
    }

    /* W6 – condición compuesta: tmp_counter++ se consume para la etiqueta
     *       ANTES de evaluar la expresión, cuyos registros intermedios
     *       podrían colisionar si se usase id como registro SSA */
    public static boolean complexCond(int a, int b) {
        if (a > 0 && b > 0) {
            return true;
        }
        return false;
    }

    public static void main(String[] args) {
        globalX = 10;
        /* W1 – literal entero 5 a parámetro double
         *      definición:  @_twice_d
         *      call-site:   @_twice_i  ← símbolo inexistente */
        double d ;
        d = twice(5);
        System.out.print(d);                /* esperado: 1.0000000000000000e+01 */

        /* W7 – literal decimal con muchos dígitos significativos
         *      strip_underscores → clean[64]; luego memmove inserta ".0"
         *      si el token original supera ~61 chars → desbordamiento */
        double big;
        big = 1.23456789012345678901234e+10;
        System.out.print(big);              /* esperado: 1.2345678901234568e+10 */

        /* W8 – string con secuencias de escape que se expanden a \XX (3 bytes)
         *      con suficientes chars no-imprimibles el buffer de 1024 bytes
         *      de strlit_llvm_content se desborda */
        System.out.print("line1\nline2\nline3\n");   /* esperado: tres líneas */

        /* W9 – Integer.parseInt(args[0]): el codegen suma +1 al índice,
         *      así args[0] accede a argv[1] (saltando el nombre del programa).
         *      Ejecutar con argumento "42" → debe imprimir 42 */
        int n;
        if (args.length > 0) {
            n = Integer.parseInt(args[0]);
            System.out.print(n);            /* esperado (con arg 42): 42 */
        }

        /* W10 – múltiples literales string distintos: el registro de strlits
         *       debe crecer correctamente y no reutilizar índices */
        System.out.print("alpha");          /* esperado: alpha   (sin \n) */
        System.out.print("beta");           /* esperado: beta    (sin \n) */
        System.out.print("gamma");          /* esperado: gamma   (sin \n) */

        /* W5 – debe devolver 99 (local), no 10 (global) */
        int r;
        r = shadowTest();
        System.out.print(r);               /* esperado: 99 */

        /* W6 – condición compuesta con &&; ambos operandos > 0 → true */
        boolean ok;
        ok = complexCond(1, 2);
        System.out.print(ok);              /* esperado: true */

        /* W11 – return explícito en main: current_ret_type debe ser i32,
         *       si queda como T_Void el codegen emite "ret void" dentro
         *       de "define i32 @main" → error de tipo en LLVM */
        return;
    }
}