#include <stdio.h>
#include <math.h>
#include "myMath.h"

// Hàm main
int main() {
    double a = 5.0, b = 3.0;
    int n = 7;

    printf("Addition: %.2f + %.2f = %.2f\n", a, b, add(a, b));
    printf("Subtraction: %.2f - %.2f = %.2f\n", a, b, subtract(a, b));
    printf("Multiplication: %.2f * %.2f = %.2f\n", a, b, multiply(a, b));
    printf("Division: %.2f / %.2f = %.2f\n", a, b, divide(a, b));

    printf("Factorial: %d! = %llu\n", n, factorial(n));
    printf("Power: %.2f ^ %.2f = %.2f\n", a, b, power(a, b));
    printf("Is %d prime? %s\n", n, is_prime(n) ? "Yes" : "No");

    return 0;
}