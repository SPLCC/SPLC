; ModuleID = 'splc auto-gen module 0'
source_filename = "splc auto-gen module 0"

declare void @write(i32)

declare i32 @read()

define i32 @mod(i32 %x, i32 %n) {
mod:
  %n2 = alloca i32, align 4
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  store i32 %n, ptr %n2, align 4
  %x3 = load i32, ptr %x1, align 4
  %x4 = load i32, ptr %x1, align 4
  %n5 = load i32, ptr %n2, align 4
  %0 = sdiv i32 %x4, %n5
  %n6 = load i32, ptr %n2, align 4
  %1 = mul i32 %0, %n6
  %2 = sub i32 %x3, %1
  ret i32 %2
}

define i32 @isPerfectNumber(i32 %number) {
isPerfectNumber:
  %j = alloca i32, align 4
  %sum = alloca i32, align 4
  %number1 = alloca i32, align 4
  store i32 %number, ptr %number1, align 4
  store i32 0, ptr %sum, align 4
  store i32 1, ptr %j, align 4
  br label %looptest

looptest:                                         ; preds = %ifcont, %isPerfectNumber
  %j2 = load i32, ptr %j, align 4
  %number3 = load i32, ptr %number1, align 4
  %0 = sdiv i32 %number3, 2
  %1 = icmp sle i32 %j2, %0
  %ifcond = icmp ne i1 %1, false
  br i1 %ifcond, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %number4 = load i32, ptr %number1, align 4
  %j5 = load i32, ptr %j, align 4
  %calltmp = call i32 @mod(i32 %number4, i32 %j5)
  %2 = icmp eq i32 %calltmp, 0
  %ifcond6 = icmp ne i1 %2, false
  br i1 %ifcond6, label %then, label %else

then:                                             ; preds = %loopbody
  %sum7 = load i32, ptr %sum, align 4
  %sum8 = load i32, ptr %sum, align 4
  %j9 = load i32, ptr %j, align 4
  %3 = add i32 %sum8, %j9
  store i32 %3, ptr %sum, align 4
  %4 = load i32, ptr %sum, align 4
  br label %ifcont

else:                                             ; preds = %loopbody
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %j10 = load i32, ptr %j, align 4
  %j11 = load i32, ptr %j, align 4
  %5 = add i32 %j11, 1
  store i32 %5, ptr %j, align 4
  %6 = load i32, ptr %j, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  %sum12 = load i32, ptr %sum, align 4
  %number13 = load i32, ptr %number1, align 4
  %7 = icmp eq i32 %sum12, %number13
  %ifcond14 = icmp ne i1 %7, false
  br i1 %ifcond14, label %then15, label %else16

then15:                                           ; preds = %afterloop
  ret i32 1

else16:                                           ; preds = %afterloop
  ret i32 0

ifcont17:                                         ; No predecessors!
  ret i32 0
}

define i32 @do_main() {
do_main:
  %i = alloca i32, align 4
  %count = alloca i32, align 4
  store i32 0, ptr %count, align 4
  store i32 1, ptr %i, align 4
  br label %looptest

looptest:                                         ; preds = %ifcont, %do_main
  %i1 = load i32, ptr %i, align 4
  %0 = icmp sle i32 %i1, 100
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %i2 = load i32, ptr %i, align 4
  %calltmp = call i32 @isPerfectNumber(i32 %i2)
  %1 = icmp eq i32 %calltmp, 1
  %ifcond3 = icmp ne i1 %1, false
  br i1 %ifcond3, label %then, label %else

then:                                             ; preds = %loopbody
  %i4 = load i32, ptr %i, align 4
  call void @write(i32 %i4)
  br label %ifcont

else:                                             ; preds = %loopbody
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %i5 = load i32, ptr %i, align 4
  %i6 = load i32, ptr %i, align 4
  %2 = add i32 %i6, 1
  store i32 %2, ptr %i, align 4
  %3 = load i32, ptr %i, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  ret i32 0
}
