#include"test.h"

int test_f(int num, int test, int expected) {
  if (test != expected)
  {
    printf("Error at test %i: returned: %i, expected: %i\n", num, test, expected);
    return -1;
  }
  return 0;
}
/*
int main() {
  int errors;

  errors = 0;
  errors -= test_f(1, 0, 1);

  printf("Errors: %i\n", errors);
  return 0;
}
*/