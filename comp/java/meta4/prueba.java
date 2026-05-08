class ClassA {
    public static int globalVar;

    /* 1. Mangling en call site vs definición */
    public static void foo(double x) {
        System.out.print(x);
    }

    /* 2. Redefinición (test adaptado a una sola clase: sobrecarga del método) */
    public static void reset() {
        globalVar = 0;
    }
    
    public static void reset(int overload) {
        globalVar = overload;
    }

    /* 4. String[] en funciones no-main */
    public static int checkArgs(String[] customArgs) {
        return customArgs.length;
    }

    /* 3. main sobrecargado para probar si tu codegen emite dos @main */
    public static void main() {
        globalVar = 1;
    }

    public static void main(String[] args) {
        int firstArg;
        int y;
        double massive;

        /* 1. Provoca crash: llamada int a función double */
        foo(5);

        /* 9. ParseArgs ambiguo */
        firstArg = Integer.parseInt(args[0]);

        /* 4. Provoca crash: .length en String[] de función no-main */
        checkArgs(args);

        /* 5 y 6. Ámbitos anidados y etiquetas tras expresiones */
        y = 1;
        int z;
        if (true) {
            
            z = 2;
            y = z;
        }

        /* 7. Desbordamiento de buffer clean[64] con literal >64 chars */
        massive = 1_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000.0;

        /* 8. Desbordamiento de buffer strlit_llvm_content[1024]. */
        System.out.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

        /* 11. Genera `ret void` dentro del método main() */
        return;
    }
}