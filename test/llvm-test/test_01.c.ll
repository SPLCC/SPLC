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

define i32 @isPalindrome(i32 %num) {
isPalindrome:
  %digit = alloca i32, align 4
  %original = alloca i32, align 4
  %reversed = alloca i32, align 4
  %num1 = alloca i32, align 4
  store i32 %num, ptr %num1, align 4
  store i32 0, ptr %reversed, align 4
  %num2 = load i32, ptr %num1, align 4
  store i32 %num2, ptr %original, align 4
  store i32 0, ptr %digit, align 4
  br label %looptest

looptest:                                         ; preds = %loopbody, %isPalindrome
  %num3 = load i32, ptr %num1, align 4
  %0 = icmp ne i32 %num3, 0
  %ifcond = icmp ne i1 %0, false
  br i1 %ifcond, label %loopbody, label %afterloop

loopbody:                                         ; preds = %looptest
  %digit4 = load i32, ptr %digit, align 4
  %num5 = load i32, ptr %num1, align 4
  %calltmp = call i32 @mod(i32 %num5, i32 10)
  store i32 %calltmp, ptr %digit, align 4
  %1 = load i32, ptr %digit, align 4
  %reversed6 = load i32, ptr %reversed, align 4
  %reversed7 = load i32, ptr %reversed, align 4
  %2 = mul i32 %reversed7, 10
  %digit8 = load i32, ptr %digit, align 4
  %3 = add i32 %2, %digit8
  store i32 %3, ptr %reversed, align 4
  %4 = load i32, ptr %reversed, align 4
  %num9 = load i32, ptr %num1, align 4
  %num10 = load i32, ptr %num1, align 4
  %5 = sdiv i32 %num10, 10
  store i32 %5, ptr %num1, align 4
  %6 = load i32, ptr %num1, align 4
  br label %looptest

afterloop:                                        ; preds = %looptest
  %reversed11 = load i32, ptr %reversed, align 4
  %original12 = load i32, ptr %original, align 4
  %7 = icmp eq i32 %reversed11, %original12
  %ifcond13 = icmp ne i1 %7, false
  br i1 %ifcond13, label %then, label %else14

then:                                             ; preds = %afterloop
  ret i32 1

else14:                                           ; preds = %afterloop
  br label %ifcont15

ifcont15:                                         ; preds = %else14
  ret i32 0
}

define i32 @do_main() {
do_main:
  %number = alloca i32, align 4
  %number1 = load i32, ptr %number, align 4
  %calltmp = call i32 @read()
  store i32 %calltmp, ptr %number, align 4
  %0 = load i32, ptr %number, align 4
  %number2 = load i32, ptr %number, align 4
  %calltmp3 = call i32 @isPalindrome(i32 %number2)
  %1 = icmp eq i32 %calltmp3, 1
  %ifcond = icmp ne i1 %1, false
  br i1 %ifcond, label %then, label %else4

then:                                             ; preds = %do_main
  call void @write(i32 1)
  br label %ifcont5

else4:                                            ; preds = %do_main
  call void @write(i32 -1)
  br label %ifcont5

ifcont5:                                          ; preds = %else4, %then
  ret i32 0
}
