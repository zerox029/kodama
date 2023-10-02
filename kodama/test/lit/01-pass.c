/**
RUN: clang %s -o %S/hello-world && %S/hello-world | FileCheck %s
CHECK: Hello world
 */

#include <stdio.h>
int main() {
  printf("Hello world\n");
  return 0;
}