declare i32 @_read(i32)
declare i32 @_write(i32)

define i32 @_mod(i32 %a, i32 %b) {
  %1 = add i32 %a, 0
  %2 = add i32 %a, 0
  %3 = add i32 %b, 0
  %4 = sdiv i32 %2, %3
  %5 = add i32 %b, 0
  %6 = mul i32 %4, %5
  %7 = sub i32 %1, %6
  ret i32 %7
}

define i32 @_main(i32 %i) {
  %1 = add i32 0, 0
  %2 = call i32 @_read(i32 %1)
  %3 = add i32 0, 0
  %4 = call i32 @_read(i32 %3)
  %5 = call i32 @_mod(i32 %2, i32 %4)
  %6 = call i32 @_write(i32 %5)
  ret i32 %6
}

define i32 @main() {
  %1 = call i32 @_main(i32 0)
  ret i32 %1
}
