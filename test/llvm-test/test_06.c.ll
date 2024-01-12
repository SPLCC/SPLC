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
  br i1 %ifcond, label %then, label %else8

then:                                             ; preds = %hanoi
  %p16 = load i32, ptr %p12, align 4
  %1 = mul i32 %p16, 1000000
  %p37 = load i32, ptr %p34, align 4
  %2 = add i32 %1, %p37
  call void @write(i32 %2)
  br label %ifcont20

else8:                                            ; preds = %hanoi
  %n9 = load i32, ptr %n1, align 4
  %3 = sub i32 %n9, 1
  %p110 = load i32, ptr %p12, align 4
  %p311 = load i32, ptr %p34, align 4
  %p212 = load i32, ptr %p23, align 4
  %calltmp = call i32 @hanoi(i32 %3, i32 %p110, i32 %p311, i32 %p212)
  %p113 = load i32, ptr %p12, align 4
  %4 = mul i32 %p113, 1000000
  %p314 = load i32, ptr %p34, align 4
  %5 = add i32 %4, %p314
  call void @write(i32 %5)
  %n15 = load i32, ptr %n1, align 4
  %6 = sub i32 %n15, 1
  %p216 = load i32, ptr %p23, align 4
  %p117 = load i32, ptr %p12, align 4
  %p318 = load i32, ptr %p34, align 4
  %calltmp19 = call i32 @hanoi(i32 %6, i32 %p216, i32 %p117, i32 %p318)
  br label %ifcont20

ifcont20:                                         ; preds = %else8, %then
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
