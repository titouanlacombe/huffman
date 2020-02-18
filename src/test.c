#include"test.h"

int test_f(int num, int test, int expected) {
	if (test != expected)
	{
		printf("Error at test %i: returned: %i, expected: %i\n", num, test, expected);
		return -1;
	}
	return 0;
}

// Return a random int a <= x < b
int randint(int a, int b)
{
	return(a + ((b - a) * (double)rand() / RAND_MAX));
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