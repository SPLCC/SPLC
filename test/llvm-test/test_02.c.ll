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
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %calculateBinomialCoefficient
  ret i32 0

else:                                             ; preds = %calculateBinomialCoefficient
  br label %ifcont

ifcont:                                           ; preds = %else
  %col5 = load i32, ptr %col2, align 4
  %row6 = load i32, ptr %row1, align 4
  %col7 = load i32, ptr %col2, align 4
  %1 = sub i32 %row6, %col7
  %2 = icmp sgt i32 %col5, %1
  %ifcond8 = icmp ne i1 %2, false
  br i1 %ifcond8, label %then9, label %else13

then9:                                            ; preds = %ifcont
  %col10 = load i32, ptr %col2, align 4
  %row11 = load i32, ptr %row1, align 4
  %col12 = load i32, ptr %col2, align 4
  %3 = sub i32 %row11, %col12
  store i32 %3, ptr %col2, align 4
  %4 = load i32, ptr %col2, align 4
  br label %ifcont14

else13:                                           ; preds = %ifcont
  br label %ifcont14

ifcont14:                                         ; preds = %else13, %then9
  br label %looptest

looptest:                                         ; preds = %loopbody, %ifcont14
  %i15 = load i32, ptr %i, align 4
  %col16 = load i32, ptr %col2, align 4
  %5 = icmp slt i32 %i15, %col16
  %ifcond17 = icmp ne i1 %5, false
  br i1 %ifcond17, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %coefficient18 = load i32, ptr %coefficient, align 4
  %coefficient19 = load i32, ptr %coefficient, align 4
  %row20 = load i32, ptr %row1, align 4
  %i21 = load i32, ptr %i, align 4
  %6 = sub i32 %row20, %i21
  %7 = mul i32 %coefficient19, %6
  store i32 %7, ptr %coefficient, align 4
  %8 = load i32, ptr %coefficient, align 4
  %coefficient22 = load i32, ptr %coefficient, align 4
  %coefficient23 = load i32, ptr %coefficient, align 4
  %i24 = load i32, ptr %i, align 4
  %9 = add i32 %i24, 1
  %10 = sdiv i32 %coefficient23, %9
  store i32 %10, ptr %coefficient, align 4
  %11 = load i32, ptr %coefficient, align 4
  %i25 = load i32, ptr %i, align 4
  %i26 = load i32, ptr %i, align 4
  %12 = add i32 %i26, 1
  store i32 %12, ptr %i, align 4
  %13 = load i32, ptr %i, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  %coefficient27 = load i32, ptr %coefficient, align 4
  ret i32 %coefficient27
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
