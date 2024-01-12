; ModuleID = 'splc auto-gen module 0'
source_filename = "splc auto-gen module 0"

declare void @write(i32)

declare i32 @read()

define i32 @calculateBinomialCoefficient(i32 %row, i32 %col) {
calculateBinomialCoefficient:
  %i = alloca i32, align 4
  %coefficient = alloca i32, align 4
  %col2 = alloca i32, align 4
  %row1 = alloca i32, align 4
  store i32 %row, ptr %row1, align 4
  store i32 %col, ptr %col2, align 4
  store i32 1, ptr %coefficient, align 4
  store i32 0, ptr %i, align 4
  %row3 = load i32, ptr %row1, align 4
  %col4 = load i32, ptr %col2, align 4
  %0 = icmp slt i32 %row3, %col4
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %then, label %else5

then:                                             ; preds = %calculateBinomialCoefficient
  ret i32 0

else5:                                            ; preds = %calculateBinomialCoefficient
  br label %ifcont6

ifcont6:                                          ; preds = %else5
  %col7 = load i32, ptr %col2, align 4
  %row8 = load i32, ptr %row1, align 4
  %col9 = load i32, ptr %col2, align 4
  %1 = sub i32 %row8, %col9
  %2 = icmp sgt i32 %col7, %1
  %ifcond10 = icmp ne i1 %2, false
  br i1 %ifcond10, label %then11, label %else15

then11:                                           ; preds = %ifcont6
  %col12 = load i32, ptr %col2, align 4
  %row13 = load i32, ptr %row1, align 4
  %col14 = load i32, ptr %col2, align 4
  %3 = sub i32 %row13, %col14
  store i32 %3, ptr %col2, align 4
  %4 = load i32, ptr %col2, align 4
  br label %ifcont16

else15:                                           ; preds = %ifcont6
  br label %ifcont16

ifcont16:                                         ; preds = %else15, %then11
  br label %looptest

looptest:                                         ; preds = %loopbody, %ifcont16
  %i17 = load i32, ptr %i, align 4
  %col18 = load i32, ptr %col2, align 4
  %5 = icmp slt i32 %i17, %col18
  %ifcond19 = icmp ne i1 %5, false
  br i1 %ifcond19, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %coefficient20 = load i32, ptr %coefficient, align 4
  %coefficient21 = load i32, ptr %coefficient, align 4
  %row22 = load i32, ptr %row1, align 4
  %i23 = load i32, ptr %i, align 4
  %6 = sub i32 %row22, %i23
  %7 = mul i32 %coefficient21, %6
  store i32 %7, ptr %coefficient, align 4
  %8 = load i32, ptr %coefficient, align 4
  %coefficient24 = load i32, ptr %coefficient, align 4
  %coefficient25 = load i32, ptr %coefficient, align 4
  %i26 = load i32, ptr %i, align 4
  %9 = add i32 %i26, 1
  %10 = sdiv i32 %coefficient25, %9
  store i32 %10, ptr %coefficient, align 4
  %11 = load i32, ptr %coefficient, align 4
  %i27 = load i32, ptr %i, align 4
  %i28 = load i32, ptr %i, align 4
  %12 = add i32 %i28, 1
  store i32 %12, ptr %i, align 4
  %13 = load i32, ptr %i, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  %coefficient29 = load i32, ptr %coefficient, align 4
  ret i32 %coefficient29
}

define i32 @do_main() {
do_main:
  %coe = alloca i32, align 4
  %c = alloca i32, align 4
  %r = alloca i32, align 4
  %r1 = load i32, ptr %r, align 4
  %calltmp = call i32 @read()
  store i32 %calltmp, ptr %r, align 4
  %0 = load i32, ptr %r, align 4
  %c2 = load i32, ptr %c, align 4
  %calltmp3 = call i32 @read()
  store i32 %calltmp3, ptr %c, align 4
  %1 = load i32, ptr %c, align 4
  %coe4 = load i32, ptr %coe, align 4
  %r5 = load i32, ptr %r, align 4
  %c6 = load i32, ptr %c, align 4
  %calltmp7 = call i32 @calculateBinomialCoefficient(i32 %r5, i32 %c6)
  store i32 %calltmp7, ptr %coe, align 4
  %2 = load i32, ptr %coe, align 4
  %coe8 = load i32, ptr %coe, align 4
  call void @write(i32 %coe8)
  ret i32 0
}
