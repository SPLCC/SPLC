; ModuleID = 'splc auto-gen module 0'
source_filename = "splc auto-gen module 0"

declare void @write(i32)

declare i32 @read()

define i32 @do_main() {
do_main:
  %c = alloca i32, align 4
  %b = alloca i32, align 4
  %a = alloca i32, align 4
  %m = alloca i32, align 4
  %tu3 = alloca i32, align 4
  %tu2 = alloca i32, align 4
  %tu1 = alloca i32, align 4
  %i = alloca i32, align 4
  %a1 = load i32, ptr %a, align 4
  store i32 1, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %b2 = load i32, ptr %b, align 4
  store i32 2, ptr %b, align 4
  %1 = load i32, ptr %b, align 4
  %c3 = load i32, ptr %c, align 4
  store i32 3, ptr %c, align 4
  %2 = load i32, ptr %c, align 4
  %tu14 = load i32, ptr %tu1, align 4
  store i32 1, ptr %tu1, align 4
  %3 = load i32, ptr %tu1, align 4
  %tu25 = load i32, ptr %tu2, align 4
  store i32 1, ptr %tu2, align 4
  %4 = load i32, ptr %tu2, align 4
  %m6 = load i32, ptr %m, align 4
  %calltmp = call i32 @read()
  store i32 %calltmp, ptr %m, align 4
  %5 = load i32, ptr %m, align 4
  %m7 = load i32, ptr %m, align 4
  %6 = icmp slt i32 %m7, 1
  %ifcond = icmp ne i1 %6, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %do_main
  call void @write(i32 0)
  br label %ifcont32

else:                                             ; preds = %do_main
  %m8 = load i32, ptr %m, align 4
  %7 = icmp eq i32 %m8, 1
  %m9 = load i32, ptr %m, align 4
  %8 = icmp eq i32 %m9, 2
  %9 = select i1 %7, i1 true, i1 %8
  %ifcond10 = icmp ne i1 %9, false
  br i1 %ifcond10, label %then11, label %else12

then11:                                           ; preds = %else
  call void @write(i32 1)
  br label %ifcont31

else12:                                           ; preds = %else
  %m13 = load i32, ptr %m, align 4
  %10 = icmp sgt i32 %m13, 2
  %ifcond14 = icmp ne i1 %10, false
  br i1 %ifcond14, label %then15, label %else30

then15:                                           ; preds = %else12
  %i16 = load i32, ptr %i, align 4
  store i32 3, ptr %i, align 4
  %11 = load i32, ptr %i, align 4
  br label %looptest

looptest:                                         ; preds = %loopbody, %then15
  %i17 = load i32, ptr %i, align 4
  %m18 = load i32, ptr %m, align 4
  %12 = icmp sle i32 %i17, %m18
  %ifcond19 = icmp ne i1 %12, false
  br i1 %ifcond19, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %tu320 = load i32, ptr %tu3, align 4
  %tu121 = load i32, ptr %tu1, align 4
  %tu222 = load i32, ptr %tu2, align 4
  %13 = add i32 %tu121, %tu222
  store i32 %13, ptr %tu3, align 4
  %14 = load i32, ptr %tu3, align 4
  %tu123 = load i32, ptr %tu1, align 4
  %tu224 = load i32, ptr %tu2, align 4
  store i32 %tu224, ptr %tu1, align 4
  %15 = load i32, ptr %tu1, align 4
  %tu225 = load i32, ptr %tu2, align 4
  %tu326 = load i32, ptr %tu3, align 4
  store i32 %tu326, ptr %tu2, align 4
  %16 = load i32, ptr %tu2, align 4
  %i27 = load i32, ptr %i, align 4
  %i28 = load i32, ptr %i, align 4
  %17 = add i32 %i28, 1
  store i32 %17, ptr %i, align 4
  %18 = load i32, ptr %i, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  %tu329 = load i32, ptr %tu3, align 4
  call void @write(i32 %tu329)
  br label %ifcont

else30:                                           ; preds = %else12
  br label %ifcont

ifcont:                                           ; preds = %else30, %afterloop
  br label %ifcont31

ifcont31:                                         ; preds = %ifcont, %then11
  br label %ifcont32

ifcont32:                                         ; preds = %ifcont31, %then
  ret i32 0
}
