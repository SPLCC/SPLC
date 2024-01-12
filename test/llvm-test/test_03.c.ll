; ModuleID = 'splc auto-gen module 0'
source_filename = "splc auto-gen module 0"

declare void @write(i32)

declare i32 @read()

define i32 @do_main() {
do_main:
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  %num = alloca i32, align 4
  %N = alloca i32, align 4
  store i32 30, ptr %N, align 4
  store i32 0, ptr %num, align 4
  store i32 1, ptr %i, align 4
  store i32 1, ptr %k, align 4
  br label %looptest

looptest:                                         ; preds = %ifcont22, %do_main
  %k1 = load i32, ptr %k, align 4
  %N2 = load i32, ptr %N, align 4
  %0 = icmp sle i32 %k1, %N2
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %loopbody, label %afterloop27

loopbody:                                         ; preds = %looptest
  br label %looptest3

looptest3:                                        ; preds = %ifcont, %loopbody
  %i4 = load i32, ptr %i, align 4
  %k5 = load i32, ptr %k, align 4
  %1 = icmp sle i32 %i4, %k5
  %ifcond6 = icmp ne i1 %1, false
  br i1 %ifcond6, label %loopbody7, label %afterloop

loopbody7:                                        ; preds = %looptest3
  %k8 = load i32, ptr %k, align 4
  %k9 = load i32, ptr %k, align 4
  %i10 = load i32, ptr %i, align 4
  %2 = sdiv i32 %k9, %i10
  %i11 = load i32, ptr %i, align 4
  %3 = mul i32 %2, %i11
  %4 = icmp eq i32 %k8, %3
  %ifcond12 = icmp ne i1 %4, false
  br i1 %ifcond12, label %then, label %else

then:                                             ; preds = %loopbody7
  %num13 = load i32, ptr %num, align 4
  %num14 = load i32, ptr %num, align 4
  %5 = add i32 %num14, 1
  store i32 %5, ptr %num, align 4
  %6 = load i32, ptr %num, align 4
  br label %ifcont

else:                                             ; preds = %loopbody7
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %i15 = load i32, ptr %i, align 4
  %i16 = load i32, ptr %i, align 4
  %7 = add i32 %i16, 1
  store i32 %7, ptr %i, align 4
  %8 = load i32, ptr %i, align 4
  br label %looptest3

afterloop:                                        ; preds = %looptest3
  %num17 = load i32, ptr %num, align 4
  %9 = icmp eq i32 %num17, 2
  %ifcond18 = icmp ne i1 %9, false
  br i1 %ifcond18, label %then19, label %else21

then19:                                           ; preds = %afterloop
  %k20 = load i32, ptr %k, align 4
  call void @write(i32 %k20)
  br label %ifcont22

else21:                                           ; preds = %afterloop
  br label %ifcont22

ifcont22:                                         ; preds = %else21, %then19
  %i23 = load i32, ptr %i, align 4
  store i32 1, ptr %i, align 4
  %10 = load i32, ptr %i, align 4
  %num24 = load i32, ptr %num, align 4
  store i32 0, ptr %num, align 4
  %11 = load i32, ptr %num, align 4
  %k25 = load i32, ptr %k, align 4
  %k26 = load i32, ptr %k, align 4
  %12 = add i32 %k26, 1
  store i32 %12, ptr %k, align 4
  %13 = load i32, ptr %k, align 4
  br label %looptest

afterloop27:                                      ; preds = %looptest
  ret i32 0
}
