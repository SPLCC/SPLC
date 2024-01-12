; ModuleID = 'splc auto-gen module 0'
source_filename = "splc auto-gen module 0"

declare void @write(i32)

declare i32 @read()

define i32 @hanoi(i32 %n, i32 %p1, i32 %p2, i32 %p3) {
hanoi:
  %p34 = alloca i32, align 4
  %p23 = alloca i32, align 4
  %p12 = alloca i32, align 4
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  store i32 %p1, ptr %p12, align 4
  store i32 %p2, ptr %p23, align 4
  store i32 %p3, ptr %p34, align 4
  %n5 = load i32, ptr %n1, align 4
  %0 = icmp eq i32 %n5, 1
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %hanoi
  %p16 = load i32, ptr %p12, align 4
  %1 = mul i32 %p16, 1000000
  %p37 = load i32, ptr %p34, align 4
  %2 = add i32 %1, %p37
  call void @write(i32 %2)
  br label %ifcont

else:                                             ; preds = %hanoi
  %n8 = load i32, ptr %n1, align 4
  %3 = sub i32 %n8, 1
  %p19 = load i32, ptr %p12, align 4
  %p310 = load i32, ptr %p34, align 4
  %p211 = load i32, ptr %p23, align 4
  %calltmp = call i32 @hanoi(i32 %3, i32 %p19, i32 %p310, i32 %p211)
  %p112 = load i32, ptr %p12, align 4
  %4 = mul i32 %p112, 1000000
  %p313 = load i32, ptr %p34, align 4
  %5 = add i32 %4, %p313
  call void @write(i32 %5)
  %n14 = load i32, ptr %n1, align 4
  %6 = sub i32 %n14, 1
  %p215 = load i32, ptr %p23, align 4
  %p116 = load i32, ptr %p12, align 4
  %p317 = load i32, ptr %p34, align 4
  %calltmp18 = call i32 @hanoi(i32 %6, i32 %p215, i32 %p116, i32 %p317)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  ret i32 0
}

define i32 @do_main() {
do_main:
  %sum = alloca i32, align 4
  store i32 3, ptr %sum, align 4
  %sum1 = load i32, ptr %sum, align 4
  %calltmp = call i32 @hanoi(i32 %sum1, i32 1, i32 2, i32 3)
  ret i32 0
}
