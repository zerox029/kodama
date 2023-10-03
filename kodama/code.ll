; ModuleID = 'KodamaGenTest'
source_filename = "KodamaGenTest"

declare i32 @printf(ptr, ...)

define i64 @main() {
entry:
  %i = alloca i64, align 8
  store i32 10, ptr %i, align 4
  %i1 = load i64, ptr %i, align 4
  %subtmp = sub i64 %i1, 5
  store i64 %subtmp, ptr %i, align 4
  %i2 = load i64, ptr %i, align 4
  ret i64 %i2
}
