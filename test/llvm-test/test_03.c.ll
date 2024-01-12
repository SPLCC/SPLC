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

looptest:                                         ; preds = %ifcont25, %do_main
  %k1 = load i32, ptr %k, align 4
  %N2 = load i32, ptr %N, align 4
  %0 = icmp sle i32 %k1, %N2
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  br label %looptest3

afterloop:                                        ; preds = %looptest
  ret i32 0

looptest3:                                        ; preds = %ifcont17, %loopbody
  %i6 = load i32, ptr %i, align 4
  %k7 = load i32, ptr %k, align 4
  %1 = icmp sle i32 %i6, %k7
  %ifcond8 = icmp ne i1 %1, false
  br i1 %ifcond8, label %loopbody4, label %afterloop5

loopbody4:                                        ; preds = %looptest3
  %k9 = load i32, ptr %k, align 4
  %k10 = load i32, ptr %k, align 4
  %i11 = load i32, ptr %i, align 4
  %2 = sdiv i32 %k10, %i11
  %i12 = load i32, ptr %i, align 4
  %3 = mul i32 %2, %i12
  %4 = icmp eq i32 %k9, %3
  %ifcond13 = icmp ne i1 %4, false
  br i1 %ifcond13, label %then, label %else16

afterloop5:                                       ; preds = %looptest3
  %num20 = load i32, ptr %num, align 4
  %5 = icmp eq i32 %num20, 2
  %ifcond21 = icmp ne i1 %5, false
  br i1 %ifcond21, label %then22, label %else24

then:                                             ; preds = %loopbody4
  %num14 = load i32, ptr %num, align 4
  %num15 = load i32, ptr %num, align 4
  %6 = add i32 %num15, 1
  store i32 %6, ptr %num, align 4
  %7 = load i32, ptr %num, align 4
  br label %ifcont17

else16:                                           ; preds = %loopbody4
  br label %ifcont17

ifcont17:                                         ; preds = %else16, %then
  %i18 = load i32, ptr %i, align 4
  %i19 = load i32, ptr %i, align 4
  %8 = add i32 %i19, 1
  store i32 %8, ptr %i, align 4
  %9 = load i32, ptr %i, align 4
  br label %looptest3

then22:                                           ; preds = %afterloop5
  %k23 = load i32, ptr %k, align 4
  call void @write(i32 %k23)
  br label %ifcont25

else24:                                           ; preds = %afterloop5
  br label %ifcont25

ifcont25:                                         ; preds = %else24, %then22
  %i26 = load i32, ptr %i, align 4
  store i32 1, ptr %i, align 4
  %10 = load i32, ptr %i, align 4
  %num27 = load i32, ptr %num, align 4
  store i32 0, ptr %num, align 4
  %11 = load i32, ptr %num, align 4
  %k28 = load i32, ptr %k, align 4
  %k29 = load i32, ptr %k, align 4
  %12 = add i32 %k29, 1
  store i32 %12, ptr %k, align 4
  %13 = load i32, ptr %k, align 4
  br label %looptest
}
