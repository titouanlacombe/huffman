#include<stdio.h>
#include"test.h"

int main(int argc, char const *argv[])
{
  int errors;

  errors = 0;
  errors -= test_f(1, 0, 1);

  printf("Errors: %i\n", errors);
  
  return 0;
}
